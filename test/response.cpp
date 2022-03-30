#include <curl/curl.h>
#include <boost/log/core.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <thread>
#include <iomanip>
#include <math.h>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
   return size * nmemb;
}

int curl_server(std::atomic<long>& total_throughput){
    CURLcode ret;
    CURL *hnd;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, "https://172.17.0.2:8081/file/example.xml/");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERPWD, "calvin:mypass");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "Some random data");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)18);
    curl_easy_setopt(hnd, CURLOPT_HTTPAUTH, (long)CURLAUTH_DIGEST);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.71.1");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CAINFO, "/usr/local/share/ca-certificates/smoothstack_root.crt");
    curl_easy_setopt(hnd, CURLOPT_SSH_KNOWNHOSTS, "/home/calvin/.ssh/known_hosts");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_data);       // write output to nothing

    ret = curl_easy_perform(hnd);

    long response_code;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &response_code);

    long size;
    curl_easy_getinfo(hnd, CURLINFO_SIZE_DOWNLOAD_T, &size);
    total_throughput.fetch_add(size);
    
    curl_easy_cleanup(hnd);
    hnd = NULL;
 
    return (int)response_code;
}

template <typename T>
float get_average(T vec){
    float sum = std::accumulate(vec.begin(),vec.end(),0);

    return sum / vec.size();
}

void threaded_curl(std::atomic<long>& total_throughput){
    std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
    double time;
    int curl_count = 0;

    while(time < 10){
        int response = curl_server(total_throughput);   // curls server and adds total_throughput
        curl_count += 1;
    
        std::chrono::steady_clock::time_point end_timer = std::chrono::steady_clock::now();
        time = std::chrono::duration<double> (end_timer - start_timer).count();
    }
}

int main(int argc, char *argv[]){
    std::vector<double> throughput;

    for(int i = 0; i < 5; i++){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::atomic<long> total_throughput = 0;
        std::vector<std::thread> threads;

        unsigned long const max_threads = 2;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);

        for(int i = 0; i < num_threads; i++){
            std::thread t(threaded_curl, std::ref(total_throughput));
            threads.push_back(std::move(t));
        }

        for(auto& t:threads){
            t.join();
        }
        double Mb = total_throughput/12500;     // convert bytes to Mb
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double time = std::chrono::duration<double> (end - begin).count();

        double Mbps = Mb/time;      // convert Mb to Mbps
        throughput.push_back(Mbps);
        //std::cout << total_throughput << std::endl;
        //std::cout << Mbps << std::endl;
    } 

    std::cout << std::fixed << std::setprecision(1) << "Average response throughput: " 
        << get_average(throughput) << " Mbps" << std::endl;
}
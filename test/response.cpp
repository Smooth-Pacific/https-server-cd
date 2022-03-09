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
    curl_easy_setopt(hnd, CURLOPT_URL, "https://[::1]:8081/file/example.xml/");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERPWD, "calvin:mypass");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "adfl;kjasdfhiuhfdv");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)18);
    curl_easy_setopt(hnd, CURLOPT_HTTPAUTH, (long)CURLAUTH_DIGEST);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.71.1");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CAINFO, "/usr/local/share/ca-certificates/smoothstack_client.crt");
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

void threaded_curl(std::atomic<int>& sleep_time, std::atomic<bool>& stop_thread_flag, std::atomic<long>& total_throughput){
    while(!stop_thread_flag){
        int response = curl_server(total_throughput);
        if(response == 200){
            if(sleep_time > 0){
                //std::cout << sleep_time << ": " << response << std::endl;
                sleep_time.fetch_sub(10);
            }
            else{
                std::cout << "reached max speed" << std::endl;
                sleep_time = 1;
                stop_thread_flag = true;
            }
        }
        else{
            //std::cout << sleep_time << ": " << response << std::endl;
            stop_thread_flag = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}

int main(int argc, char *argv[]){
    std::vector<double> throughput;

    for(int i = 0; i < 5; i++){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::atomic<int> sleep_time = 500;               // time in milliseconds
        std::atomic<bool> stop_thread_flag = false;     // if true stop threads
        std::atomic<long> total_throughput = 0;
        std::vector<std::thread> threads;

        unsigned long const max_threads = 8;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);

        for(int i = 0; i < num_threads; i++){
            std::thread t(threaded_curl, std::ref(sleep_time), std::ref(stop_thread_flag), std::ref(total_throughput));
            threads.push_back(std::move(t));
        }

        for(auto& t:threads){
            t.join();
        }

        double Mb = total_throughput/125;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double time = std::chrono::duration<double> (end - begin).count();

        double Mbps = Mb/time;
        //std::cout << "Run " << i << ": " << Mbps << std::endl;
        throughput.push_back(Mbps);
    } 
    
    //for(auto response:responses){
    //    std::cout << response << std::endl;
    //}

    std::cout << std::fixed << std::setprecision(1) << "Average response throughput: " << get_average(throughput) << " Mbps" << std::endl;
}
#include <curl/curl.h>
#include <boost/log/core.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <thread>
#include <iomanip>
#include <math.h>

//char* id;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
   return size * nmemb;
}

int curl_server(){
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
    
    curl_easy_cleanup(hnd);
    hnd = NULL;
 
    return (int)response_code;
}

template <typename T>
float get_average(T vec){
    float sum = std::accumulate(vec.begin()+1,vec.end(),0);

    return sum / vec.size();
}

void threaded_curl(std::atomic<int>& sleep_time, std::atomic<bool>& stop_thread_flag){
    while(!stop_thread_flag){
        int response = curl_server();
        if(response == 200){
            if(sleep_time > 0){
                //std::cout << sleep_time << ": " << response << std::endl;
                sleep_time -= 10;
            }
            else{
                std::cout << "reached max speed" << std::endl;
                sleep_time = 1;
                stop_thread_flag = true;
            }
        }
        else{
            std::cout << sleep_time << ": " << response << std::endl;
            stop_thread_flag = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}

int main(int argc, char *argv[]){
    std::vector<double> responses;

    for(int i = 0; i < 5; i++){
        std::atomic<int> sleep_time = 500;               // time in milliseconds
        std::atomic<bool> stop_thread_flag = false;     // if true stop threads
        std::vector<std::thread> threads;

        unsigned long const max_threads = 8;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);

        for(int i = 0; i < num_threads; i++){
            std::thread t(threaded_curl, std::ref(sleep_time), std::ref(stop_thread_flag));
            threads.push_back(std::move(t));
        }

        for(auto& t:threads){
            t.join();
        }
    
        double rate = (sleep_time/100000.0)/2.0;
        responses.push_back(pow(rate, -1));
    } 
    
    //for(auto response:responses){
    //    std::cout << response << std::endl;
    //}

    std::cout << std::fixed << std::setprecision(0) << "Average response rate: " << get_average(responses) << " req/s" << std::endl;
}
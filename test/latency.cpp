#include <curl/curl.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
   return size * nmemb;
}

int curl_server(){
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
    
    curl_easy_cleanup(hnd);
    hnd = NULL;
 
    return (int)response_code;
}

template <typename T>
float get_average(T vec){
    float sum = std::accumulate(vec.begin(),vec.end(),0);

    return sum / vec.size();
}

int main(int argc, char *argv[]){
    std::vector<int> latency;
    double time;
    std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();

    while(time < 10){
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        int response = curl_server();
        if(response == 200){
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            //std::cout << response << std::endl;
            latency.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count());
        }
        else{
            //std::cout << response << std::endl;
        }

        std::chrono::steady_clock::time_point end_timer = std::chrono::steady_clock::now();
        time = std::chrono::duration<double> (end_timer - start_timer).count();
    }

    //for(auto lat:latency){
    //    std::cout << lat << std::endl;
    //}
    std::cout << "Average latency usage (milliseconds): " << get_average(latency) << "ms" << std::endl;
}
#include <iostream>
#include <string.h>
#include <sys/times.h>
#include <sys/vtimes.h>
#include <thread>
#include <vector>
#include <numeric>
#include <iomanip>

clock_t lastCPU, lastSysCPU, lastUserCPU;
int numProcessors;
unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

int parse_line(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while(*p <'0' || *p > '9'){
        p++;
    }
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

// returns data in KB
int get_mem_data(const char* path, const char* line_name){
    FILE* file = fopen(path, "r");
    int result = -1;
    char line[128];

    while(fgets(line, 128, file) != NULL){
        if(strncmp(line, line_name, 6) == 0){
            result = parse_line(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void init(){
    FILE* file_process_cpu;
    struct tms timeSample;
    char line[128];

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file_process_cpu = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while(fgets(line, 128, file_process_cpu) != NULL){
        if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }
    fclose(file_process_cpu);

    FILE* file_cpu = fopen("/proc/stat", "r");
    fscanf(file_cpu, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file_cpu);
}

double get_process_cpu(){
    struct tms timeSample;
    clock_t now;
    double percent;

    now = times(&timeSample);
    if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
        timeSample.tms_utime < lastUserCPU){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        percent = (timeSample.tms_stime - lastSysCPU) +
            (timeSample.tms_utime - lastUserCPU);
        percent /= (now - lastCPU);
        percent /= numProcessors;
        percent *= 100;
    }
    lastCPU = now;
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    return percent;
}

double get_cpu(){
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

template <typename T>
float get_average(T vec){
    float sum = std::accumulate(vec.begin()+1,vec.end(),0);

    return sum / vec.size();
}

int main(){
    init();
    double time;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    std::vector<float> cpu_usage, free_ram, free_swap;
    while(time < 10){

        cpu_usage.push_back(get_cpu());
        free_ram.push_back(get_mem_data("/proc/meminfo", "MemAvailable:"));
        free_swap.push_back(get_mem_data("/proc/meminfo", "SwapFree:"));
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time = std::chrono::duration<double> (end - begin).count();
    }

    //for(auto cpu:cpu_usage){
    //    std::cout << cpu << std::endl;
    //}
    
    std::cout << "Average CPU usage (10s): " << get_average(cpu_usage) << "%" << std::endl;
    std::cout << std::fixed << "Average RAM availability (10s): " << get_average(free_ram) << "KB" << std::endl;
    std::cout << std::fixed << "Average SWAP availability (10s): " << get_average(free_swap) << "KB" << std::endl;
}
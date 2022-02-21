class Performance_Monitoring{
    private:
        void monitor();

        uint16_t parse_line(char* line);
        uint16_t get_process_ram_usage();
        std::atomic<bool> stop_thread_flag;

    public:
        Performance_Monitoring(std::atomic<bool>& stop_thread_flag);
        void run();
};
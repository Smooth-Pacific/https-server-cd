class Performance_Monitoring{
    private:
        void monitor();

        uint16_t parse_line(char* line);
        uint16_t get_process_ram_usage();

    public:
        Performance_Monitoring();
        void run();
};
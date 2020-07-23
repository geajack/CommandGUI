#include <string>

class Process
{
        int startTime;
        bool running;
    public:
        Process(std::string command);
        void run();
        void stop();
        bool isRunning();
        std::string getOutput();
};
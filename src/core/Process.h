#include <string>

class Process
{
        int startTime;
    public:
        Process(std::string command);
        void run();
        bool isRunning();
        std::string getOutput();
};
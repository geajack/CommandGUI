#include <string>

static const int BUFFER_SIZE = 1024;

enum ProcessRunResult
{
    OKAY,
    FAILED
};

class Process
{
        int startTime;
        bool running;
        std::string command;
        int outputStream;
        unsigned int childPID;
        std::string output;
        char outputBuffer[BUFFER_SIZE];
    public:
        Process(std::string command);
        ProcessRunResult run();
        void stop();
        bool isRunning();
        std::string getOutput();
};
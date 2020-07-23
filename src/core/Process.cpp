#include "Process.h"
#include <ctime>

Process::Process(std::string command)
{
    running = false;
}

void Process::run()
{
    startTime = std::time(NULL);
    running = true;
}

void Process::stop()
{
    running = false;
}

bool Process::isRunning()
{
    return running;
}

std::string Process::getOutput()
{
    std::string output = "";
    int secondsElapsed = std::time(NULL) - startTime;
    while (output.size() < secondsElapsed)
    {
        output += ".";
    }
    return output;
}
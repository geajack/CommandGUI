#include "Process.h"
#include <ctime>

Process::Process(std::string command)
{
    
}

void Process::run()
{
    startTime = std::time(NULL);
}

bool Process::isRunning()
{
    return true;
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
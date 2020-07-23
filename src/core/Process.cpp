#include "Process.h"
#include <ctime>
#include <unistd.h>

Process::Process(std::string command)
{
    running = false;
    this->command = command;
}

ProcessRunResult Process::run()
{
    startTime = std::time(NULL);
    running = true;
    output = "";

    int pipeEndpoints[2];
    pipe(pipeEndpoints);

    unsigned int forkResult = fork();

    if (forkResult == -1)
    {
        return FAILED;
    }
    else
    {
        if (forkResult > 0)
        {
            childPID = forkResult;
            outputStream = pipeEndpoints[0];
            close(pipeEndpoints[1]);
            return OKAY;
        }
        else
        {
            close(pipeEndpoints[0]);
            dup2(pipeEndpoints[1], STDOUT_FILENO);
            dup2(pipeEndpoints[1], STDERR_FILENO);
            //execlp("echo", "echo", "hello");
            printf("%s", "hello world\n");
            
            exit(1);
            return OKAY;
        }
    }
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
    if (running)
    {
        int result = read(outputStream, outputBuffer, BUFFER_SIZE - 1);
        if (result == -1)
        {
            return "Failed to read stdout!";
        }
        else
        {
            if (result == 0)
            {
                running = false;
            }
            else
            {
                int bytesRead = result;
                outputBuffer[bytesRead] = 0;
                output += outputBuffer;
            }
        }
    }
    return output;
}
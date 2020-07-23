#include "Process.h"
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <string.h>

typedef struct Pipe
{
    int readEndpoint;
    int writeEndpoint;
} Pipe;


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

    Pipe outputPipe;
    pipe((int*) &outputPipe);

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
            outputStream = outputPipe.readEndpoint;
            close(outputPipe.writeEndpoint);
            return OKAY;
        }
        else
        {
            close(outputPipe.readEndpoint);
            dup2(outputPipe.writeEndpoint, STDOUT_FILENO);
            dup2(outputPipe.writeEndpoint, STDERR_FILENO);
            execlp("sh", "sh", "-c", command.c_str(), NULL);

            exit(1);
            return OKAY;
        }
    }
}

void Process::stop()
{
    kill(childPID, SIGINT);
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
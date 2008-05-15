#include "gameServer.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

bool gRunning;

void signalHandler(int signal)
{
    gRunning = false;
}


int main(int argc, char** argv)
{
    gRunning = true;
    
    signal(SIGINT, signalHandler);
    
    GameServer server;

    char const* address = "0.0.0.0";
    short port = 7707;
    
    printf("Starting server on %s:%d...\n", address, port);
    
    if(server.start(address, port) < 0)
    {
        printf("Start failed\n");
        return 0;
    }

    printf("Started.\n");
    
    while(gRunning)
    {
        sleep(1);
    }
    
    printf("Stopping server...\n");

    server.stop();

    printf("Stopped.\n");

    return 0;
}

    

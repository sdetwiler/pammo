#include "types_platform.h"

#include "gameServer.h"

#include <stdio.h>
#include <signal.h>

bool gRunning;

void signalHandler(int signal)
{
    gRunning = false;
}


int main(int argc, char** argv)
{
#ifdef WIN32
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(ret!= NO_ERROR)
    {
        printf("Failed to initialize networking.\n");
        return 1;
    }
#endif

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
        usleep(1000);
    }
    
    printf("Stopping server...\n");

    server.stop();

    printf("Stopped.\n");

    return 0;
}

    

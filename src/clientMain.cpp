#include "gameClient.h"

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
    
    GameClient client;

    char const* address = "127.0.0.1";
    short port = 7707;
    
    printf("Client connecting to %s:%d...\n", address, port);
    
    if(client.connect(address, port) < 0)
    {
        printf("Connect failed\n");
        return 0;
    }

    printf("Connected.\n");

    MapInstanceId id;
    id.mServerId = 1;
    id.mMapId = 1;
    id.mInstanceId = 1;
    
    client.login(id);
    
    
    while(client.isConnected() && gRunning)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    
    printf("Disconnecting client...\n");

    client.disconnect();

    printf("Stopped.\n");

    return 0;
}

    

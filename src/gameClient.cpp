#include "gameClient.h"

GameClient::GameClient()
{
}

GameClient::~GameClient()
{
}


void GameClient::onClientConnected(Client* client, Connection* connection)
{
    mSession = new Session(connection);
    mSession->setObserver(this);
}

void GameClient::onClientDisconnected(Client* client, Connection* connection)
{
    delete mSession;
    mSession = NULL;
}

void GameClient::onCommand(Session* session, Command* command)
{
    printf("GameClient::onCommand\n");
}


void GameClient::onSessionClosed(Session* sesson)
{
    printf("GameClient::onSessionClosed\n");
}



int GameClient::connect(char const* address, short port)
{
    mClient.setObserver(this);
    return mClient.connect(address, port);
}

int GameClient::disconnect()
{
    return mClient.disconnect();
}



#include "gameServer.h"


GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}


void GameServer::onNewConnection(Server* server, Connection* connection)
{
    Session* session = new Session(connection);
    session->setObserver(this);
    mSessions[connection] = session;
}

void GameServer::onConnectionClosed(Server* server, Connection* connection)
{
    ConnectionSessionMap::iterator i = mSessions.find(connection);
    if(i == mSessions.end())
        return;

    delete i->second;
    mSessions.erase(i);
}

void GameServer::onCommand(Session* session, Command* command)
{
    printf("GameServer::onCommand\n");
}


void GameServer::onSessionClosed(Session* sesson)
{
    printf("GameServer::onSessionClosed\n");
}



int GameServer::start(char const* address, short port)
{
    mServer.setObserver(this);
    return mServer.start(address, port);
}

int GameServer::stop()
{
    return mServer.stop();
}



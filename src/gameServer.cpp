#include "gameServer.h"


GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}


void GameServer::onNewConnection(Server* server, Connection* connection)
{
    printf("GameServer::onNewConnection\n");
    
    Session* session = new Session(connection);
    session->setObserver(this);
    mSessions[connection] = session;
}

void GameServer::onConnectionClosed(Server* server, Connection* connection)
{
    printf("GameServer::onConnectionClosed\n");
    
    ConnectionSessionMap::iterator i = mSessions.find(connection);
    if(i == mSessions.end())
        return;

    delete i->second;
    mSessions.erase(i);
}

void GameServer::onCommand(Session* session, Command* command)
{
    printf("GameServer::onCommand\n");
    switch(command->getId())
    {
    case PAMMO_COMMAND_LOGIN:
        onLogin(session, (LoginCommand*)command);
        break;

    default:
        printf("GameServer received unknown command %d\n", command->getId());
        CommandFactory::deleteCommand(command);
    }
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


void GameServer::onLogin(Session* session, LoginCommand* cmd)
{
    session->send(CommandFactory::newCommand(PAMMO_COMMAND_STATUSUPDATE));
}

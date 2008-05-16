#include "gameServer.h"
#include "serverMap.h"

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}


void GameServer::onNewConnection(Server* server, Connection* connection)
{
    printf("GameServer::onNewConnection\n");
    
    ServerSession* session = new ServerSession(connection);
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
        onLogin((ServerSession*)session, (LoginCommand*)command);
        break;

    default:
        printf("GameServer received unknown command %d\n", command->getId());
        CommandFactory::deleteCommand(command);
    }
}


void GameServer::onSessionClosed(Session* session)
{
    printf("GameServer::onSessionClosed\n");
    ServerSession* ss = (ServerSession*)session;
    if(ss->getServerMap())
    {
        ss->getServerMap()->onSessionClosed(ss);
    }
}



int GameServer::start(char const* address, short port)
{
    int ret;
    
    mServer.setObserver(this);

    ret = mMapIndex.start("mappack.xml");
    if(ret < 0)
        return ret;

    return mServer.start(address, port);
}

int GameServer::stop()
{
    int ret;

    ret = mServer.stop();

    ret = mMapIndex.stop();

    return ret;
}


void GameServer::onLogin(ServerSession* session, LoginCommand* cmd)
{
    printf("GameServer::onLogin\n");

    // Find map that is being logged into.
    ServerMap* map = mMapIndex.getServerMap(cmd->getMapInstanceId());
    if(!map)
    {
        CommandFactory::deleteCommand(cmd);

        ErrorCommand* errorCmd = (ErrorCommand*)CommandFactory::newCommand(PAMMO_COMMAND_ERROR);
        errorCmd->setError(PAMMO_ERROR_UNKNOWNMAP);
        session->send(errorCmd);
        return;
    }

    // Login into the map.
    map->onLogin(session, cmd);
}

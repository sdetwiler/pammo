#include "serverMap.h"

ServerMap::ServerMap(uint16_t serverId)
{
    mInstanceId.mServerId = serverId;
    mInstanceId.mMapId = 0;
    mInstanceId.mInstanceId = 0;

    mNextPlayerId = 0;
}


ServerMap::~ServerMap()
{}

MapInstanceId const& ServerMap::getInstanceId() const
{
    return mInstanceId;
}

void ServerMap::setInstanceId(MapInstanceId const& id)
{
    mInstanceId = id;
}

int ServerMap::load(char const* filename, uint32_t instanceId)
{
    printf("TODO: ServerMap::load\n");

    mInstanceId.mInstanceId = instanceId;
    
    printf("Loaded %s\n", filename);
    return 0;
}

void ServerMap::onLogin(ServerSession* session, LoginCommand* cmd)
{
    printf("ServerMap::onLogin\n");
    
    SessionPlayerMap::iterator i = mPlayers.find(session);
    if(i!= mPlayers.end())
    {
        ErrorCommand* errCmd = (ErrorCommand*)CommandFactory::newCommand(PAMMO_COMMAND_ERROR);
        errCmd->setError(PAMMO_ERROR_ALREADYLOGGEDIN);
        session->send(errCmd);

        CommandFactory::deleteCommand(cmd);
        return;
    }

    session->setServerMap(this);
    
    Player* player = new Player(session, mNextPlayerId++);

    // player->setVehicle(cmd->getVehicle());
    // etc.
    
    CommandFactory::deleteCommand(cmd);

    mPlayers[session] = player;

    player->send(CommandFactory::newCommand(PAMMO_COMMAND_STATUSUPDATE));
}

void ServerMap::onSessionClosed(ServerSession* session)
{
    printf("ServerMap::onSessionClosed\n");

    SessionPlayerMap::iterator i = mPlayers.find(session);
    if(i!=mPlayers.end())
    {
        delete i->second;
        mPlayers.erase(i);
    }
}



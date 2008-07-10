#include "gameClient.h"

GameClient::GameClient()
{
    mSession = NULL;
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
    if(mSession)
        delete mSession;
    mSession = NULL;
}

void GameClient::onCommand(Session* session, Command* command)
{
    printf("GameClient::onCommand\n");
    switch(command->getId())
    {
    case PAMMO_COMMAND_STATUSUPDATE:
        onStatusUpdate((StatusUpdateCommand*)command);
        break;

    case PAMMO_COMMAND_ERROR:
        onError((ErrorCommand*)command);
        break;

    default:
        printf("GameClient received unknown command %d\n", command->getId());
        CommandFactory::deleteCommand(command);
    }
}


void GameClient::onSessionClosed(Session* sesson)
{
    printf("GameClient::onSessionClosed\n");
    mConnected = false;
}

int GameClient::connect(char const* address, short port)
{
    mClient.setObserver(this);
    mConnected = true;
    int ret = mClient.connect(address, port);
    if(ret < 0)
    {
        mConnected = false;
        return ret;
    }
    
    return ret;
}

bool GameClient::isConnected()
{
    return mConnected;
}

int GameClient::disconnect()
{
    return mClient.disconnect();
}


void GameClient::login(MapInstanceId const& id)
{
    printf("GameClient::login\n");
    LoginCommand* cmd = (LoginCommand*)CommandFactory::newCommand(PAMMO_COMMAND_LOGIN);
    cmd->setMapInstanceId(id);
    mSession->send(cmd);
}


void GameClient::onStatusUpdate(StatusUpdateCommand* cmd)
{
    printf("GameClient::onStatusUpdate\n");

    // TODO: Deserialize command and set status.
    
    CommandFactory::deleteCommand(cmd);
}

void GameClient::onError(ErrorCommand* cmd)
{
    printf("GameClient::onError: %d\n", cmd->getError());
    CommandFactory::deleteCommand(cmd);

    disconnect();
}

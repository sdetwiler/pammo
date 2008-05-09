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

    login();
}

void GameClient::onClientDisconnected(Client* client, Connection* connection)
{
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

    default:
        printf("GameClient received unknown command %d\n", command->getId());
        CommandFactory::deleteCommand(command);
    }
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


void GameClient::login()
{
    mSession->send(CommandFactory::newCommand(PAMMO_COMMAND_LOGIN));
}


void GameClient::onStatusUpdate(StatusUpdateCommand* cmd)
{
    printf("GameClient::onStatusUpdate\n");

    // TODO: Deserialize command and set status.
    
    CommandFactory::deleteCommand(cmd);
}

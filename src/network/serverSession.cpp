#include "serverSession.h"

ServerSession::ServerSession(Connection* connection) 
    : Session(connection)
{
    mServerMap = NULL;
}


ServerSession::~ServerSession()
{}


void ServerSession::setServerMap(ServerMap* map)
{
    mServerMap = map;
}

ServerMap* ServerSession::getServerMap() const
{
    return mServerMap;
}

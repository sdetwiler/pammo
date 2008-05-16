#include "types.h"
#include "command.h"
#include <stdlib.h>
#include <arpa/inet.h>

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////

Command::Command()
{
    mObserver = 0;
}

Command::~Command()
{

}

void Command::setObserver(CommandObserver* o)
{
    mObserver = o;
}


CommandObserver* Command::getObserver()
{
    return mObserver;
}

///////////////////////////////////////////////////////////////////////////////

LoginCommand::LoginCommand()
{}

LoginCommand::~LoginCommand()
{}

int LoginCommand::serialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    *((uint16_t*)(data)) = htons(mMapInstanceId.mServerId);
    *((uint16_t*)(data+sizeof(uint16_t))) = htons(mMapInstanceId.mMapId);
    *((uint32_t*)(data+sizeof(uint16_t)+sizeof(uint16_t))) = htonl(mMapInstanceId.mInstanceId);
    
    return 0;
}

int LoginCommand::deserialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    mMapInstanceId.mServerId = ntohs(*((uint16_t*)(data)));
    mMapInstanceId.mMapId = ntohs(*((uint16_t*)(data+sizeof(uint16_t))));
    mMapInstanceId.mInstanceId = ntohl(*((uint32_t*)(data+sizeof(uint16_t)+sizeof(uint16_t))));

    return 0;
}

MapInstanceId const& LoginCommand::getMapInstanceId() const
{
    return mMapInstanceId;
}

void LoginCommand::setMapInstanceId(MapInstanceId const& id)
{
    mMapInstanceId = id;
}


///////////////////////////////////////////////////////////////////////////////

StatusUpdateCommand::StatusUpdateCommand()
{}

StatusUpdateCommand::~StatusUpdateCommand()
{}

int StatusUpdateCommand::serialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;
    
    return 0;
}

int StatusUpdateCommand::deserialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

ErrorCommand::ErrorCommand()
{
    mError = 0;
}

ErrorCommand::~ErrorCommand()
{}

int ErrorCommand::serialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    *((int32_t*)data) = htonl(mError);
    
    return 0;
}

int ErrorCommand::deserialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    mError = ntohl(*((int32_t*)data));
        
    return 0;
}

void ErrorCommand::setError(int32_t error)
{
    mError = error;
}

int32_t ErrorCommand::getError()
{
    return mError;
}


///////////////////////////////////////////////////////////////////////////////

Command* CommandFactory::newCommand(uint32_t id)
{
    switch(id)
    {
    case PAMMO_COMMAND_LOGIN:
        return new LoginCommand;

    case PAMMO_COMMAND_STATUSUPDATE:
        return new StatusUpdateCommand;
        
    case PAMMO_COMMAND_ERROR:
        return new ErrorCommand;

    default:
        return NULL;
    }
}


void CommandFactory::deleteCommand(Command* command)
{
    delete command;
}

///////////////////////////////////////////////////////////////////////////////

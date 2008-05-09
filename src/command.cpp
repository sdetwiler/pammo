#include "command.h"
#include <stdlib.h>

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
    
    return 0;
}

int LoginCommand::deserialize(uint8_t* data, uint32_t dataLen)
{
    if(dataLen != getPayloadLength())
        return -1;

    return 0;
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

Command* CommandFactory::newCommand(uint32_t id)
{
    switch(id)
    {
    case PAMMO_COMMAND_LOGIN:
        return new LoginCommand;

    case PAMMO_COMMAND_STATUSUPDATE:
        return new StatusUpdateCommand;
        
    default:
        return NULL;
    }
}


void CommandFactory::deleteCommand(Command* command)
{
    delete command;
}

///////////////////////////////////////////////////////////////////////////////

#include "command.h"
#include <stdlib.h>

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


Command* CommandFactory::newCommand(uint32_t id)
{
    return NULL;
}


void CommandFactory::deleteCommand(Command* command)
{
    delete command;
}


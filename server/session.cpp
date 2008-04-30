#include <stdio.h>

#include "session.h"




Session::Session(Connection* connection)
{
    mObserver = 0;
    mInState = Header;
    mInOffset = 0;
    mInCommand = 0;
    
    mConnection = connection;
    mConnection->setObserver(this);
}


Session::~Session()
{
    //
}

void Session::setObserver(SessionObserver* o)
{
    mObserver = o;
}

SessionObserver* Session::getObserver()
{
    return mObserver;
}

void Session::onReadable(Connection* connection)
{
    printf("Session::onReadable\n");
    read();
}

void Session::onWritable(Connection* connection)
{
    printf("Session::onWritable\n");
    write();
}

void Session::onError(Connection* connection)
{
    printf("Session::onError\n");
    mConnection->close();
}

void Session::onClosed(Connection* connection)
{
    printf("Session::onClosed\n");
    mConnection = NULL;
}

void Session::read()
{
    int ret;
    
    if(mConnection->isReadable() == false)
    {
        printf("Session::read called but not readable\n");
        return;
    }
    

    uint32_t toRead;
    uint32_t numRead;
    
    if(mInState == Header)
    {
        toRead = SIZEOF_PROTOCOL_HEADER - mInOffset;
        
        ret = mConnection->read(mInHeader+mInOffset, toRead, numRead);
        if(ret < 0)
        {
            return;
        }
        
        mInOffset+= numRead;
        if(toRead != numRead)
        {
            return;
        }
        
        mInState = Payload;
        mInProtocolVersion = ntohs(*((uint16_t*)(mInHeader+PROTOCOL_VERSION)));
        mInCommandId = ntohl(*((uint32_t*)(mInHeader+PROTOCOL_COMMANDID)));
        mInPayloadLen = ntohl(*((uint32_t*)(mInHeader+PROTOCOL_PAYLOADLEN)));
        mInOffset = 0;
        
        mInCommand = CommandFactory::newCommand(mInCommandId);
        if(!mInCommand)
        {
            printf("Unknown command id %d\n", mInCommandId);
            mConnection->close();
            return;
        }

        mInPayload = new uint8_t[mInPayloadLen];
        
    }

    while(mInState == Payload)
    {
        toRead = mInPayloadLen - mInOffset;
        ret = mConnection->read(mInPayload + mInOffset, toRead, numRead);
        if(ret < 0)
        {
            return;
        }
        
        mInOffset+= numRead;
        if(toRead != numRead)
        {
            return;
        }
        
        ret = mInCommand->deserialize(mInPayload, mInPayloadLen);
        if(ret < 0)
        {
            printf("Command failed to deserialize payload.\n");
            mConnection->close();
            return;
        }
    }

    if(mObserver)
    {
        mObserver->onCommand(this, mInCommand);
        mInCommand = NULL;
    }
    else
    {
        CommandFactory::deleteCommand(mInCommand);
        mInCommand = NULL;
    }
}

void Session::write()
{
    if(mConnection->isWritable() == false)
        return;
}

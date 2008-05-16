#include <stdio.h>

#include "session.h"




Session::Session(Connection* connection)
{
    mObserver = 0;
    mInState = Header;
    mInOffset = 0;
    mInCommand = 0;
    mInPayload = NULL;
    mInPayloadLen = 0;

    mOutState = Header;
    mOutOffset = 0;
    mOutPayload = NULL;
    mOutPayloadLen = 0;
    mOutCommand = NULL;
    
    mConnection = connection;
    mConnection->setObserver(this);
}


Session::~Session()
{
    if(mObserver)
        mObserver->onSessionClosed(this);
    
    if(mConnection)
        mConnection->setObserver(NULL);
}

void Session::setObserver(SessionObserver* o)
{
    mObserver = o;
}

SessionObserver* Session::getObserver()
{
    return mObserver;
}

void Session::send(Command* command)
{
    mCommands.push(command);
    mConnection->notifyOwner();
}

void Session::onReadable(Connection* connection)
{
    //    printf("Session::onReadable\n");
    read();
}

void Session::onWritable(Connection* connection)
{
    //    printf("Session::onWritable\n");
    write();
}

void Session::onError(Connection* connection)
{
    //    printf("Session::onError\n");
    mConnection->close();
}

void Session::onClosed(Connection* connection)
{
    //    printf("Session::onClosed\n");
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

//        printf("Session::read have header:\n\tprotocol: %d\n\tcommandId: %u\n\tpayloadLen: %u\n",
//               mInProtocolVersion, mInCommandId, mInPayloadLen);
        
        mInCommand = CommandFactory::newCommand(mInCommandId);
        if(!mInCommand)
        {
            printf("Unknown command id %d\n", mInCommandId);
            mConnection->close();
            return;
        }

        if(mInPayloadLen)
            mInPayload = new uint8_t[mInPayloadLen];
        
    }

    while(mInState == Payload)
    {
        //        printf("Session::read state is payload\n");
        
        toRead = mInPayloadLen - mInOffset;
        if(toRead)
        {
            
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
        }

        mInState = Header;
    }

    ret = mInCommand->deserialize(mInPayload, mInPayloadLen);
    if(ret < 0)
    {
        printf("Command failed to deserialize payload.\n");
        delete[] mInPayload;
        
        mConnection->close();
        return;
    }

    if(mInPayload)
    {
        delete[] mInPayload;
        mInPayload = 0;
    }
    
    
    mInOffset = 0;

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
    //    printf("Session::write\n");
    
    int ret;
    
    if(mConnection->isWritable() == false)
    {
        printf("Session::write not writable\n");
        return;
    }
    

    if(!mOutCommand)
    {
        //        printf("Session::write nothing to write.\n");
        
        if(mCommands.size() == 0)
        {
            return;
        }
        //        printf("Session::write starting send of new command\n");
        
        mOutCommand = mCommands.front();
        mCommands.pop();

        mOutState = Header;
        mOutOffset = 0;
        mOutPayloadLen = mOutCommand->getPayloadLength();
        mOutPayload = new uint8_t[mOutPayloadLen];
        ret = mOutCommand->serialize(mOutPayload, mOutPayloadLen);
        if(ret < 0)
        {
            printf("Command failed to serialize: %d\n", ret);
            CommandFactory::deleteCommand(mOutCommand);
            delete[] mOutPayload;
            mOutCommand = 0;
            return;
        }

        ret = ProtocolHeader::serialize(mOutHeader, sizeof(mOutHeader),
                                  mOutCommand->getId(), mOutPayloadLen);
        if(ret < 0)
        {
            printf("Failed to serialize protocol header: %d\n", ret);
            CommandFactory::deleteCommand(mOutCommand);
            mOutCommand = 0;
            return;
        }
    }

    uint32_t toWrite;
    uint32_t numWritten;
    
    if(mOutState == Header)
    {
        toWrite = SIZEOF_PROTOCOL_HEADER - mOutOffset;

        ret = mConnection->write(mOutHeader + mOutOffset, toWrite, numWritten);
        if(ret < 0)
        {
            return;
        }

        mOutOffset+= numWritten;
        if(toWrite != numWritten)
        {
            return;
        }

        mOutOffset = 0;
        mOutState = Payload;
    }
    
    if(mOutState == Payload)
    {
        toWrite = mOutPayloadLen - mOutOffset;
        ret = mConnection->write(mOutPayload+mOutOffset, toWrite, numWritten);
        if(ret < 0)
        {
            return;
        }

        mOutOffset+= numWritten;
        if(toWrite != numWritten)
        {
            return;
        }

        // Wrote everything.
        mOutState = Header;
        delete[] mOutPayload;
    }
    
    if(mOutCommand->getObserver())
    {
        mOutCommand->getObserver()->onSent(mOutCommand);
    }

    CommandFactory::deleteCommand(mOutCommand);
    mOutCommand = NULL;
}

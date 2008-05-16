#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "connection.h"

Connection::Connection(ConnectionOwner* owner)
{
    mOwner = owner;
    mObserver = 0;
    mSocket = 0;
    mReadable = false;
    mWritable = false;
}

Connection::~Connection()
{
    if(mSocket)
        ::close(mSocket);

}

void Connection::setObserver(ConnectionObserver* o)
{
    mObserver = o;
}

ConnectionObserver* Connection::getObserver()
{
    return mObserver;
}

void Connection::onReadable()
{
    //    printf("Connection::onReadable\n");
    
    mReadable = true;
    
    if(mObserver)
        mObserver->onReadable(this);
}

void Connection::onWritable()
{
    //    printf("Connection::onWritable\n");
    mWritable = true;

    if(mObserver)
        mObserver->onWritable(this);
}

int Connection::read(uint8_t* buf, uint32_t bufLen, uint32_t& numRead)
{
    //    printf("Connection::read on %d\n", mSocket);
    
    numRead = 0;
    
    if(!mReadable)
        return -1;
    
    ssize_t read = recv(mSocket, buf, bufLen, 0);
    //    printf("recv returned %d\n", read);

    if(read == 0)
    {
        //        printf("Connection::read of zero\n");
        
        // Note that this makes a callback into the ConnectionObserver who probably called read...
        close();
        return -1;
    }
    
    else if(read < 0)
    {
        int e = errno;
        //printf("recv failed: %s (%d)\n", strerror(e), e);
        mReadable = false;
        return -1;
    }

    // No longer readable.
    if(read < bufLen)
        mReadable = false;

    numRead = read;
    
    return 0;
}


int Connection::write(uint8_t* buf, uint32_t bufLen, uint32_t& numWritten)
{
    //    printf("Connection::write on %d\n", mSocket);
    numWritten = 0;
    
    if(!mWritable)
        return -1;
    
    ssize_t sent = send(mSocket, buf, bufLen, 0);
    if(sent < 0)
    {
        int e = errno;
        //        printf("send failed: %s (%d)", strerror(e), e);
        mWritable = false;
        return -1;
    }
    if(sent < bufLen)
    {
        mWritable = false;
    }

    numWritten = sent;
    return 0;
}

void Connection::close()
{
    mOwner->closeConnection(this);
}

void Connection::notifyOwner()
{
    mOwner->notify();
}

void Connection::setSocket(int socket)
{
    mSocket = socket;
}

int Connection::getSocket()
{
    return mSocket;
}

void Connection::setAddress(struct sockaddr_in const* addr)
{
    memcpy(&mAddress, addr, sizeof(struct sockaddr_in));
}

struct sockaddr_in const* Connection::getAddress()
{
    return& mAddress;
}

bool Connection::isReadable()
{
    return mReadable;
}


bool Connection::isWritable()
{
    return mWritable;
}

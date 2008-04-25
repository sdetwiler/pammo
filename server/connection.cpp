#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "server.h"

Connection::Connection(Server* server)
{
    mServer = server;
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

int Connection::read(uint8_t* buf, uint32_t bufLen, uint32_t& numRead)
{
    numRead = 0;
    
    if(!mReadable)
        return -1;
    
    ssize_t read = recv(mSocket, buf, bufLen, 0);
    if(read == 0)
    {
        // Note that this makes a callback into the ConnectionObserver who probably called read...
        close();
        return -1;
    }
    
    else if(read < 0)
    {
        int e = errno;
        printf("recv failed: %s (%d)", strerror(e), e);
        mReadable = false;
        return e;
    }

    // No longer readable.
    if(read < bufLen)
        mReadable = false;

    numRead = read;
    
    return 0;
}


int Connection::write(uint8_t* buf, uint32_t bufLen, uint32_t& numWritten)
{
    numWritten = 0;
    
    if(!mWritable)
        return -1;
    
    ssize_t sent = send(mSocket, buf, bufLen, 0);
    if(sent < 0)
    {
        int e = errno;
        printf("recv failed: %s (%d)", strerror(e), e);
        mWritable = false;
        return e;
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
    mServer->closeConnection(this);
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

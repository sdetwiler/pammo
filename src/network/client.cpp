#include "types.h"
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#include "client.h"

Client::Client()
{
    mObserver = 0;
    mSocket = 0;
    mNotifySocket = 0;
    mHighFd = 0;
    mThreadInitialized = false;
    
    mRunning = false;
    mConnection = NULL;
}


Client::~Client()
{
    if(mConnection)
        delete mConnection;
}


int Client::connect(char const* address, short port)
{
    int ret;

    // socket
    mSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);
    
    // connect.
    ret = ::connect(mSocket, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("connect failed\n");
        disconnect();        
        return ret;
    }

    // Notify socket setup.
    mNotifySocket = socket(PF_INET, SOCK_DGRAM, 0);
    addr.sin_port = 0;

    ret = bind(mNotifySocket, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Get notify socket address.
    int32_t addrLen = sizeof(mNotifyAddr);
    ret = getsockname(mNotifySocket, (struct sockaddr*)&mNotifyAddr, &addrLen);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Add socket to poller.
    ret = addSocket(mSocket, READ|WRITE);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Add notify socket to poller.
    ret = addSocket(mNotifySocket, READ);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Create thread.
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    mRunning = true;

    ret = pthread_create(&mThread, &attr, threadBootFunc, this);
    if(ret < 0)
    {
        mRunning = false;
        disconnect();
        return ret;
    }
    mThreadInitialized = true;

    pthread_attr_destroy(&attr);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    return 0;
}


int Client::disconnect()
{
    if(mRunning)
    {
        mRunning = false;

        notify();

        if(mThreadInitialized)
            pthread_join(mThread, NULL);
        mThreadInitialized = false;
    }
    
    if(mSocket)
        closesocket(mSocket);  // stupid win32

    if(mNotifySocket)
        closesocket(mNotifySocket);

    mSocket = 0;
    mNotifySocket = 0;

    if(mObserver)
        mObserver->onClientDisconnected(this, mConnection);
 
    return 0;
}

void Client::setObserver(ClientObserver* o)
{
    mObserver = o;
}

ClientObserver* Client::getObserver()
{
    return mObserver;
}

void Client::setConnectionEventNotification(Connection* connection, int events)
{
    modifySocket(connection->getSocket(), events);
}

//////////////////////////////////////////////////

void Client::modifySocket(SOCKET s, int events)
{
    if(events & READ)
        FD_SET(s, &mReadFds);
    else
        FD_CLR(s, &mReadFds);

    if(events & WRITE)
        FD_SET(s, &mWriteFds);
    else
        FD_CLR(s, &mWriteFds);
}

int Client::addSocket(SOCKET const& s, int events)
{
    if(s > mHighFd)
        mHighFd = s;

    int ret;
    if(events & READ)
        FD_SET(s, &mReadFds); 
    if(events & WRITE)
        FD_SET(s, &mWriteFds);

    // Make non-blocking.

#ifdef WIN32
    u_long val = 1;
    ret = ioctlsocket(s, FIONBIO, &val);
#else
    int flags;
    flags = fcntl(s, F_GETFL, 0);
    if(flags < 0)
    {
        return ret;
    }
    ret = fcntl(s, F_SETFL, flags | O_NONBLOCK);
    if(ret < 0)
    {
        return ret;
    }
#endif

    return 0;
}

int Client::removeSocket(SOCKET const& s)
{
    FD_CLR(s, &mReadFds);
    FD_CLR(s, &mWriteFds);

    if(s == mHighFd)
    {
        // TODO: Find new high fd.
    }
    return 0;
}

void Client::notify()
{
    int ret;
    int foo = 0;
    ret = sendto(mNotifySocket, (char const*)&foo, sizeof(foo), 0, (struct sockaddr*)&mNotifyAddr, sizeof(mNotifyAddr));
}

void* Client::threadBootFunc(void* arg)
{
    ((Client*)arg)->threadFunc();
    return NULL;
}

void Client::threadFunc()
{
    printf("Client::threadFunc\n");

    mConnection = new Connection(this);
    mConnection->setSocket(mSocket);
    
    if(mObserver)
        mObserver->onClientConnected(this, mConnection);

    while(1)
    {
        fd_set readFds = mReadFds;
        fd_set writeFds = mWriteFds;
        fd_set errorFds;
        FD_ZERO(&errorFds);

        // Cast to in for win32 compatibility.
        int count = select((int)mHighFd, &readFds, &writeFds, &errorFds, NULL);
        for(int i=0; i<count; ++i)
        {
            if(FD_ISSET(i, &readFds))
            {
                if(i == mNotifySocket)
                {
                    int foo;
                    recv(i, (char*)&foo, sizeof(foo), 0);

                    // Check if should shut down.
                    if(!mRunning)
                    {
                        return;
                    }
                }

                mConnection->onReadable();
            }
            if(FD_ISSET(i, &writeFds))
            {
                mConnection->onWritable();
            }
        }
    }
    /*****    
    // SCD should use a sane size.
    uint32_t numEvents = 16;
    struct epoll_event* events = new struct epoll_event[16];

    while(1)
    {
        //        printf("pre epoll_wait\n");
        ret = epoll_wait(mPoller, events, numEvents, -1);
        if(ret < 0)
        {
            printf("epoll_wait failed %d\n", ret);
            return;
        }
        //        printf("post epoll_wait\n");

        for(uint32_t i=0; i<ret; ++i)
        {
            int s = events[i].data.fd;
            
            // If the notify socket was written to.
            if(s == mNotifySocket)
            {
                int foo;
                recv(s, &foo, sizeof(foo), 0);

                // Check if should shut down.
                if(!mRunning)
                {
                    delete[] events;
                    return;
                }

                // If notified, force a write on the connection.  This
                // is in case the socket was readable or writable but
                // no more data was available. The notification
                // arrives when new data is ready to write.

                mConnection->onWritable();
            }

            else
            {
                if(events[i].events & EPOLLERR)
                {
                    //mConnection->onError();
                }

                if(events[i].events & EPOLLIN)
                {
                    mConnection->onReadable();
                }

                if(events[i].events & EPOLLOUT)
                {
                    if(mConnection) // Connection could have closed from inside onReadable.
                        mConnection->onWritable();
                }
            }
        }
    }
    
    delete[] events;
    ****/
}


void Client::closeConnection(Connection* connection)
{
    printf("Client::closeConnection\n");
    
    if(mObserver)
    {
        mObserver->onClientDisconnected(this, mConnection);
    }

    removeSocket(mSocket);
    mSocket = 0;
    delete mConnection;
    mConnection = 0;
}


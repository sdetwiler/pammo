#include "types_platform.h"

#include <stdio.h>
#include <pthread.h>

#include "server.h"

Server::Server()
{
    mObserver = 0;
    mSocket = 0;
    mNotifySocket = 0;
    mRunning = false;

    FD_ZERO(&mReadFds);
    FD_ZERO(&mWriteFds);

    //    pthread_mutex_init(&mLock, NULL);
}


Server::~Server()
{
    stop();

    //    pthread_mutex_destroy(&mLock);
}

void Server::setObserver(ServerObserver* o)
{
    mObserver = o;
}

ServerObserver* Server::getObserver()
{
    return mObserver;
}

int Server::start(char const* address, short port)
{
    int ret;
    
    // socket
    mSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);
    
    // bind
    ret = bind(mSocket, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("bind failed\n");
        stop();        
        return ret;
    }

    // Notify socket setup.
    mNotifySocket = socket(PF_INET, SOCK_DGRAM, 0);
    addr.sin_port = 0;

    ret = bind(mNotifySocket, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Get notify socket address.
    int addrLen = sizeof(mNotifyAddr);
    ret = getsockname(mNotifySocket, (struct sockaddr*)&mNotifyAddr, &addrLen);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Add listen socket to poller.
    ret = addSocket(mSocket, READ);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Add notify socket to poller.
    ret = addSocket(mNotifySocket, READ);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Create thread.
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    mRunning = true;

    ret = pthread_create(&mThread, &attr, threadBootFunc, this);
    if(ret < 0)
    {
        mRunning = false;
        stop();
        return ret;
    }

    pthread_attr_destroy(&attr);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    return 0;
}

//////////////////////////////////////////////////

int Server::stop()
{
    printf("Server::stop\n");
    
    if(mRunning)
    {
        mRunning = false;

        notify();
        
        pthread_join(mThread, NULL);
    }
    
    
    for(SocketConnectionMap::iterator i=mConnections.begin();
        i!=mConnections.end();
        i=mConnections.begin()) // Because calling close removes the connection from the map, reset.
    {
        printf("%d connections to cleanup\n", mConnections.size());
        i->second->close();
        printf("post close\n");
        
    }

    mConnections.clear();

    if(mSocket)
        closesocket(mSocket);

    if(mNotifySocket)
        closesocket(mNotifySocket);

    mSocket = 0;
    mNotifySocket = 0;
    
    return 0;
}

void Server::setConnectionEventNotification(Connection* connection, int events)
{
    modifySocket(connection->getSocket(), events);
}

//////////////////////////////////////////////////

void Server::modifySocket(SOCKET s, int events)
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

//////////////////////////////////////////////////

int Server::addSocket(SOCKET s, int events)
{
    if(s > mHighFd)
        mHighFd = s;

    int ret;

    modifySocket(s, events);

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

//////////////////////////////////////////////////

int Server::removeSocket(SOCKET s)
{
    FD_CLR(s, &mReadFds);
    FD_CLR(s, &mWriteFds);

    if(s == mHighFd)
    {
        // TODO: Find new high fd.
    }
    return 0;
}

//////////////////////////////////////////////////

void Server::notify()
{
    int ret;
    int foo = 0;
    ret = sendto(mNotifySocket, (char*)&foo, sizeof(foo), 0, (struct sockaddr*)&mNotifyAddr, sizeof(mNotifyAddr));
}

//////////////////////////////////////////////////

void* Server::threadBootFunc(void* arg)
{
    ((Server*)arg)->threadFunc();
    return NULL;
}

//////////////////////////////////////////////////

void Server::threadFunc()
{
    int ret;

    printf("Server::threadFunc\n");

    // listen
    ret = listen(mSocket, 128);
    if(ret < 0)
    {
        printf("listen failed %d\n", ret);
        return;
    }

    while(1)
    {
        fd_set readFds = mReadFds;
        fd_set writeFds = mWriteFds;
        fd_set errorFds;
        FD_ZERO(&errorFds);
        
        SocketConnectionMap::iterator it;

        printf("%d connections\n", mConnections.size());

        // Cast to in for win32 compatibility.
        int count = select((int)mHighFd, &readFds, &writeFds, &errorFds, NULL);
        printf("select returns with %d\n", count);

        if(FD_ISSET(mNotifySocket, &readFds))
        {
            int foo;
            recv(it->first, (char*)&foo, sizeof(foo), 0);

            // Check if should shut down.
            if(!mRunning)
            {
                return;
            }
            --count;
        }
        if(FD_ISSET(mSocket, &readFds))
        {
            onNewConnection();
            --count;
        }

        if(count)
        {
            for(it = mConnections.begin(); it != mConnections.end(); ++it)
            {
                if(FD_ISSET(it->first, &readFds))
                {
                    it->second->onReadable();
                }

                if(FD_ISSET(it->first, &writeFds))
                {
                    it->second->onWritable();
                }
            }
        }
    }

/**
    int ret;
    
    // listen
    ret = listen(mSocket, 128);
    if(ret < 0)
    {
        printf("listen failed %d\n", ret);
        return;
    }

    // SCD should use a sane size.
    uint32_t numEvents = 16;
    struct epoll_event* events = new struct epoll_event[16];

    while(1)
    {
        printf("%d connections\n", mConnections.size());
        ret = epoll_wait(mPoller, events, numEvents, -1);
        if(ret < 0)
        {
            printf("epoll_wait failed %d\n", ret);
            return;
        }

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
            }

            // If new connection.
            else if(s == mSocket)
            {
                //                pthread_mutex_lock(&mLock);
                onNewConnection();
                //                pthread_mutex_unlock(&mLock);
            }

            else
            {
                //                pthread_mutex_lock(&mLock);
                IntConnectionMap::iterator it;
                if(events[i].events & EPOLLERR)
                {
                    // Need to search each time. A function may
                    // close connection and remove it from the
                    // map.
                    it = mConnections.find(s);
                    if(it != mConnections.end())
                    {
                        //it->second->onError();
                    }
                }

                if(events[i].events & EPOLLIN)
                {
                    it = mConnections.find(s);
                    if(it != mConnections.end())
                    {
                        it->second->onReadable();
                    }
                }

                if(events[i].events & EPOLLOUT)
                {
                    it = mConnections.find(s);
                    if(it != mConnections.end())
                    {
                        it->second->onWritable();
                    }
                }
            }

            //           pthread_mutex_unlock(&mLock);
        }
    }
    
    delete[] events;
    ****/
}

//////////////////////////////////////////////////

int Server::onNewConnection()
{
    //    printf("onNewConnection\n");
    
    // While mSocket is readable, accept any pending connections.
    while(true)
    {
        Connection* conn = new Connection(this);  
        struct sockaddr_in addr;
        int addrLen = sizeof(addr);
        SOCKET newSock = accept(mSocket, (struct sockaddr*)&addr, &addrLen);
        // Cast for win32 compatibility.
        if(((int)newSock) < 0)
        {
            //   closeConnection(conn);
            delete conn;

            int e = getSocketError();
            switch(e)
            {
            case EWOULDBLOCK:
                return 0;

            default:
                return e;
            }
        }

        printf("New connection: %d\n", newSock);
        
        conn->setSocket(newSock);
        conn->setAddress(&addr);
        
        addSocket(conn->getSocket(), READ|WRITE);
        mConnections[conn->getSocket()] = conn;

        if(mObserver)
            mObserver->onNewConnection(this, conn);
    }
    
    return 0;
}


//////////////////////////////////////////////////

void Server::closeConnection(Connection* c)
{
    printf("Server::closeConnection %d\n", c->getSocket());
    
    removeSocket(c->getSocket());
    mConnections.erase(c->getSocket());
    //    close(c->getSocket());
    
    //    printf("closed: %d. %d connections still open\n", c->getSocket(), mConnections.size());

    // Notify connection observer.
    if(c->getObserver())
        c->getObserver()->onClosed(c);

    // Notify server observer.
    if(mObserver)
        mObserver->onConnectionClosed(this, c);
            
    delete c;
}

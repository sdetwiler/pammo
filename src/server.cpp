#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "server.h"

Server::Server()
{
    mObserver = 0;
    mSocket = 0;
    mNotifySocket = 0;
    mPoller = 0;
    mRunning = false;

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

    // Create epoll.
    mPoller = epoll_create(16);
    if(mPoller < 0)
    {
        stop();
        return ret;
    }
    
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
    uint32_t addrLen = sizeof(mNotifyAddr);
    ret = getsockname(mNotifySocket, (struct sockaddr*)&mNotifyAddr, &addrLen);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Add listen socket to poller.
    ret = addSocket(mSocket, EPOLLIN);
    if(ret < 0)
    {
        stop();
        return ret;
    }

    // Add notify socket to poller.
    ret = addSocket(mNotifySocket, EPOLLIN);
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
    if(mRunning)
    {
        mRunning = false;

        notify();
        
        pthread_join(mThread, NULL);
    }
    
    if(mSocket)
        close(mSocket);

    if(mNotifySocket)
        close(mNotifySocket);

    if(mPoller)
        close(mPoller);

    for(IntConnectionMap::iterator i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        i->second->close();
    }
    
    mSocket = 0;
    mNotifySocket = 0;
    mPoller = 0;
    
    return 0;
}

//////////////////////////////////////////////////

int Server::addSocket(int s, int events)
{
    int ret;

    // Make non-blocking.
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
    
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    
    event.events = events|EPOLLET;
    event.data.fd = s;
    
    ret = epoll_ctl(mPoller, EPOLL_CTL_ADD, s, &event);
    if(ret < 0)
    {
        return ret;
    }

    return 0;
}

//////////////////////////////////////////////////

int Server::removeSocket(int s)
{
    int ret;
    
    ret = epoll_ctl(mPoller, EPOLL_CTL_DEL, s, NULL);
    if(ret < 0)
    {
        return ret;
    }

    return 0;
}

//////////////////////////////////////////////////

void Server::notify()
{
    int ret;
    int foo = 0;
    ret = sendto(mNotifySocket, &foo, sizeof(foo), 0, (struct sockaddr*)&mNotifyAddr, sizeof(mNotifyAddr));
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
}

//////////////////////////////////////////////////

int Server::onNewConnection()
{
    printf("onNewConnection\n");
    
    // While mSocket is readable, accept any pending connections.
    while(true)
    {
        Connection* conn = new Connection(this);  
        struct sockaddr_in addr;
        socklen_t addrLen = sizeof(addr);
        int newSock = accept(mSocket, (struct sockaddr*)&addr, &addrLen);
        if(newSock < 0)
        {
            int e = errno;
            switch(e)
            {
            case EWOULDBLOCK:
                return 0;

            default:
                closeConnection(conn);
                return e;
            }
        }

        printf("New connection: %d\n", newSock);
        
        conn->setSocket(newSock);
        conn->setAddress(&addr);
        
        addSocket(conn->getSocket(), EPOLLIN|EPOLLOUT);
        mConnections[conn->getSocket()] = conn;

        if(mObserver)
            mObserver->onNewConnection(this, conn);
    }
    
    return 0;
}


//////////////////////////////////////////////////

void Server::closeConnection(Connection* c)
{
    removeSocket(c->getSocket());
    mConnections.erase(c->getSocket());
    close(c->getSocket());
    
    printf("closed: %d\n", c->getSocket());

    // Notify connection observer.
    if(c->getObserver())
        c->getObserver()->onClosed(c);

    // Notify server observer.
    if(mObserver)
        mObserver->onConnectionClosed(this, c);
            
    delete c;
}

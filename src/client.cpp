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

#include "client.h"

Client::Client()
{
    mObserver = 0;
    mSocket = 0;
    mNotifySocket = 0;
}


Client::~Client()
{

}


int Client::connect(char const* address, short port)
{
    int ret;

    // Create epoll.
    mPoller = epoll_create(16);
    if(mPoller < 0)
    {
        disconnect();
        return ret;
    }
    
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
    uint32_t addrLen = sizeof(mNotifyAddr);
    ret = getsockname(mNotifySocket, (struct sockaddr*)&mNotifyAddr, &addrLen);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Add socket to poller.
    ret = addSocket(mSocket, EPOLLIN|EPOLLOUT);
    if(ret < 0)
    {
        disconnect();
        return ret;
    }

    // Add notify socket to poller.
    ret = addSocket(mNotifySocket, EPOLLIN);
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
        
        pthread_join(mThread, NULL);
    }
    
    if(mSocket)
        close(mSocket);

    if(mNotifySocket)
        close(mNotifySocket);

    if(mPoller)
        close(mPoller);

    mSocket = 0;
    mNotifySocket = 0;
    mPoller = 0;

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

int Client::addSocket(int s, int events)
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

int Client::removeSocket(int s)
{
    int ret;
    
    ret = epoll_ctl(mPoller, EPOLL_CTL_DEL, s, NULL);
    if(ret < 0)
    {
        return ret;
    }

    return 0;
}

void Client::notify()
{
    int ret;
    int foo = 0;
    ret = sendto(mNotifySocket, &foo, sizeof(foo), 0, (struct sockaddr*)&mNotifyAddr, sizeof(mNotifyAddr));
}

void* Client::threadBootFunc(void* arg)
{
    ((Client*)arg)->threadFunc();
    return NULL;
}

void Client::threadFunc()
{
    printf("Client::threadFunc\n");
    
    int ret;
    
    mConnection = new Connection(this);
    mConnection->setSocket(mSocket);
    
    if(mObserver)
        mObserver->onClientConnected(this, mConnection);
    
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
                    mConnection->onWritable();
                }
            }
        }
    }
    
    delete[] events;
}


void Client::closeConnection(Connection* connection)
{
    if(mObserver)
    {
        mObserver->onClientDisconnected(this, mConnection);
    }

    mSocket = 0;
    delete mConnection;
    mConnection = 0;
}


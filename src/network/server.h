#ifndef SERVER_H
#define SERVER_H

#include "types_platform.h"
#include <pthread.h>
#include <map>

#include "connection.h"
#include "session.h"

class Server;


class ServerObserver
{
    public:
        ServerObserver()
        {}

        virtual ~ServerObserver()
        {}

        virtual void onNewConnection(Server* server, Connection* connection)=0;
        virtual void onConnectionClosed(Server* server, Connection* connection)=0;
};

class Server :
    public ConnectionOwner
{
    public:
        Server();
        ~Server();

        int start(char const* address, short port);
        int stop();

        void setObserver(ServerObserver* o);
        ServerObserver* getObserver();

        virtual void setConnectionEventNotification(Connection* connection, int events);
        virtual void closeConnection(Connection* c);
        virtual void notify();
        
    protected:
    private:

        enum SocketEvent
        {
            READ = 1,
            WRITE = 2
        };

        int addSocket(SOCKET s, int events);
        int removeSocket(SOCKET s);
        void modifySocket(SOCKET s, int events);

        static void* threadBootFunc(void* arg);
        void threadFunc();

        int onNewConnection();

        ServerObserver* mObserver;
        
        SOCKET mSocket;
        SOCKET mNotifySocket;
        struct sockaddr_in mNotifyAddr;
        
//        int mPoller;
        fd_set mReadFds;
        fd_set mWriteFds;
        SOCKET mHighFd;        

        //        pthread_mutex_t mLock;
        typedef std::map< SOCKET, Connection* > SocketConnectionMap;
        SocketConnectionMap mConnections;
        
        pthread_t mThread;
        bool mRunning;
};

#endif // SERVER_H

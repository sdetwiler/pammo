#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

class Server
{
    public:
        Server();
        ~Server();

        int start(char const* address, short port);
        int stop();

        void setObserver(ServerObserver* o);
        ServerObserver* getObserver();

        void closeConnection(Connection* c);
        
    protected:
    private:

        int addSocket(int s, int events);
        int removeSocket(int s);

        static void* threadBootFunc(void* arg);
        void threadFunc();

        int onNewConnection();

        void notify();
        
        ServerObserver* mObserver;
        
        int mSocket;
        int mNotifySocket;
        struct sockaddr_in mNotifyAddr;
        
        
        int mPoller;

        //        pthread_mutex_t mLock;
        typedef std::map< int, Connection* > IntConnectionMap;
        IntConnectionMap mConnections;
        
        pthread_t mThread;
        bool mRunning;
};

#endif // SERVER_H

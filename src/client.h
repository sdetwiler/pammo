#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>

#include "connection.h"
#include "session.h"

class Client;


class ClientObserver
{
    public:
        ClientObserver()
        {}

        virtual ~ClientObserver()
        {}

        virtual void onClientConnected(Client* client, Connection* connection)=0;
        virtual void onClientDisconnected(Client* client, Connection* connection)=0;
};

class Client :
    public ConnectionOwner
{
    public:
        Client();
        ~Client();

        int connect(char const* address, short port);
        int disconnect();

        void setObserver(ClientObserver* o);
        ClientObserver* getObserver();
        virtual void closeConnection(Connection* connection);
        
    protected:
    private:

        int addSocket(int s, int events);
        int removeSocket(int s);

        static void* threadBootFunc(void* arg);
        void threadFunc();

        void notify();
        
        ClientObserver* mObserver;

        Connection* mConnection;
        
        int mSocket;
        int mNotifySocket;
        struct sockaddr_in mNotifyAddr;
        
        int mPoller;
        
        pthread_t mThread;
        bool mRunning;
};

#endif // CLIENT_H

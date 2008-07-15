#ifndef CLIENT_H
#define CLIENT_H
#include "types.h"

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
        virtual void notify();
        
    protected:
    private:

        enum SocketEvent
        {
            READ = 1,
            WRITE = 2
        };
        int addSocket(SOCKET const& s, int events);
        int removeSocket(SOCKET const& s);

        static void* threadBootFunc(void* arg);
        void threadFunc();

        ClientObserver* mObserver;

        Connection* mConnection;
        
        SOCKET mSocket;
        SOCKET mNotifySocket;
        struct sockaddr_in mNotifyAddr;
        
        //int mPoller;
        fd_set mReadFds;
        fd_set mWriteFds;
        SOCKET mHighFd;        


        pthread_t mThread;
        bool mThreadInitialized;
        bool mRunning;
};

#endif // CLIENT_H

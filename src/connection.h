#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Connection;
class Server;


class ConnectionOwner
{
    public:
        ConnectionOwner()
        {}

        virtual ~ConnectionOwner()
        {}

        virtual void closeConnection(Connection* connection)=0;
        virtual void notify()=0;
};

class ConnectionObserver
{
    public:
        ConnectionObserver()
        {}

        virtual ~ConnectionObserver()
        {}

        virtual void onReadable(Connection* connection) = 0;
        virtual void onWritable(Connection* connection) = 0;
        virtual void onError(Connection* connection) = 0;
        virtual void onClosed(Connection* connection) = 0;
};


class Connection
{
    public:
        Connection(ConnectionOwner* owner);
        ~Connection();

        void setObserver(ConnectionObserver* o);
        ConnectionObserver* getObserver();

        int read(uint8_t* buffer, uint32_t bufLen, uint32_t& numRead);
        int write(uint8_t* buffer, uint32_t bufLen, uint32_t& numWritten);

        bool isReadable();
        bool isWritable();

        void onReadable();
        void onWritable();
        
        // Calling this function will destroy this object.
        void close();

        // Tell connection to notify it's owner.
        void notifyOwner();
        
        void setSocket(int socket);
        int getSocket();

        void setAddress(struct sockaddr_in const* addr);
        struct sockaddr_in const* getAddress();
        
        
    protected:
    private:
        ConnectionOwner* mOwner;
        
        ConnectionObserver* mObserver;
        
        struct sockaddr_in mAddress;
        int mSocket;

        bool mReadable;
        bool mWritable;
};

#endif // CONNECTION_H

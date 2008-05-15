#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include "session.h"

class ServerMap;

class ServerSession
    : public Session
{
    public:
        ServerSession(Connection* connection);
        virtual ~ServerSession();

        void setServerMap(ServerMap* map);
        ServerMap* getServerMap() const;

    private:
        ServerMap* mServerMap;
};


#endif // SERVERSESSION_H

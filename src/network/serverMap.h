#ifndef SERVERMAP_H
#define SERVERMAP_H


#include "types.h"
#include "player.h"
#include "serverSession.h"

#include <map>


// Server's representation of a map.
class ServerMap
{
    public:
        ServerMap(uint16_t serverId);
        virtual ~ServerMap();

        
        int load(char const* filename, uint32_t instanceId);

        void setInstanceId(MapInstanceId const& id);
        
        // Returns map id.
        MapInstanceId const& getInstanceId() const;

        void onLogin(ServerSession* session, LoginCommand* cmd);
        void onSessionClosed(ServerSession* session);
        
    protected:

    private:

        MapInstanceId mInstanceId;

        typedef std::map< ServerSession*, Player* > SessionPlayerMap;
        // Connected players.
        SessionPlayerMap mPlayers;

        // Next player id that's unique to this map.
        uint32_t mNextPlayerId;
};


#endif // SERVERMAP_H

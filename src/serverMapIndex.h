#ifndef SERVERMAPINDEX_H
#define SERVERMAPINDEX_H

#include <map>
#include "serverMap.h"

class ServerMapIndex
{
    public:
        ServerMapIndex();
        virtual ~ServerMapIndex();

        // Loads all maps defined in mapPackFile.
        int start(char const* filename);
        int stop();
        uint32_t getServerId() const;

        void addServerMap(ServerMap* map);
        
        ServerMap* getServerMap(MapInstanceId const& id);

        uint32_t getMapCount() const;
        
        
    protected:

    private:

        uint32_t mServerId;
        bool mRunning;
        
        typedef std::map< MapInstanceId, ServerMap* > MapInstanceIdServerMapMap;
        MapInstanceIdServerMapMap mMaps;
};


#endif // SERVERMAPINDEX_H

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
        uint32_t getServerId();

        ServerMap* getServerMap(MapInstanceId const& id);
        
    protected:

    private:

        uint32_t mServerId;
        bool mRunning;
        
        typedef std::map< MapInstanceId, ServerMap* > MapInstanceIdServerMapMap;
        MapInstanceIdServerMapMap mMaps;
};


#endif // SERVERMAPINDEX_H

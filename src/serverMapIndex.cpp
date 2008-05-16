#include "serverMapIndex.h"

ServerMapIndex::ServerMapIndex()
{
    mServerId = 0;
    mRunning = false;
}

ServerMapIndex::~ServerMapIndex()
{
    stop();
}

int ServerMapIndex::start(char const* filename)
{
    int rc;
    
    printf("TODO: ServerMapIndex::load\n");

    mServerId = 0; // Read assigned serverid from file.

    ServerMap* serverMap;
    
    // HACK: Make up server id for now.
    mServerId = 1;
    
    // For each map defined in file
    //  serverMap = new ServerMap(getServerId());
    //  serverMap->load(mapFilename);
    //  addServerMap(serverMap);


    // HACK: Make up a map for now.
    MapInstanceId id;
    id.mServerId = getServerId();
    id.mMapId = 1;
    id.mInstanceId = 1;

    serverMap = new ServerMap(getServerId());
    serverMap->setInstanceId(id);
    addServerMap(serverMap);
    
    
    printf("Loaded %s\n", filename);

    printf("%d active maps.\n", getMapCount());
    
    mRunning = true;
    
    return 0;
    
}


int ServerMapIndex::stop()
{
    if(!mRunning)
        return 0;
    
    //
    // shutdown all ServerMaps
    // delete all ServerMaps
    for(MapInstanceIdServerMapMap::iterator i=mMaps.begin(); i!=mMaps.end(); ++i)
    {
        delete i->second;
    }

    mMaps.clear();
    
    return 0;
}

ServerMap* ServerMapIndex::getServerMap(MapInstanceId const& id)
{
    MapInstanceIdServerMapMap::iterator i = mMaps.find(id);
    if(i == mMaps.end())
        return NULL;

    return i->second;
}

void ServerMapIndex::addServerMap(ServerMap* map)
{
    mMaps[map->getInstanceId()] = map;
}

uint32_t ServerMapIndex::getServerId() const
{
    return mServerId;
}


uint32_t ServerMapIndex::getMapCount() const
{
    return mMaps.size();
}



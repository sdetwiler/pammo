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
     
    // For each map defined in file
    //  ServerMap* serverMap = new ServerMap(getServerId());
    //  serverMap->load(mapFilename);
    //  mMaps.insert(serverMap->getInstanceId(), serverMap);
    
    printf("Loaded %s\n", filename);

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

    return 0;
}

ServerMap* ServerMapIndex::getServerMap(MapInstanceId const& id)
{
    MapInstanceIdServerMapMap::iterator i = mMaps.find(id);
    if(i == mMaps.end())
        return NULL;

    return i->second;
}

    


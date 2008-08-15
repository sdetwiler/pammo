#include "pammo.h"
#include "world.h"
#include "levelLoader.h"

namespace pammo
{

LevelLoader::LevelLoader()
{
    
}

LevelLoader::~LevelLoader()
{

}


bool LevelLoader::parseMapName(char* s, char* mapName)
{
    int column = 1;
    s=strtok(s, ",\"");
    while(s)
    {
        switch(column)
        {
        case 1: // Map name
            strcpy(mapName, s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}

bool LevelLoader::parseSpawnEvent(char* s, SpawnEvent* evt)
{
    int column = 1;
    s=strtok(s, ",\"");
    while(s)
    {
        switch(column)
        {
        case 1: // Start time
            evt->mStartTime = atol(s)*1000000;
            break;

        case 2: // Duration
            evt->mDuration = atol(s)*1000000;
            break;

        case 3: // Enemy count
            evt->mCount = atol(s);
            break;

        case 4: // Enemy name
            strcpy(evt->mEnemyName, s);
            break;

        case 5: // Spawn point
            evt->mSpawnId = atol(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}

void LevelLoader::dumpSpawnEvent(SpawnEvent* evt)
{
    dprintf("\
SpawnEvent\n\
  StartTime:  %u\n\
  Duration:   %u\n\
  Frequency:  %u\n\
  EnemyName:  %s\n\
  EnemyCount: %u",
    (uint32_t)(evt->mStartTime/1000000),
    (uint32_t)(evt->mDuration/1000000),
    (uint32_t)(evt->mFreq/1000000),
    evt->mEnemyName,
    evt->mCount);
};

bool LevelLoader::load(char const* filename, char* mapName)
{
    // open file.
    FILE* f = fopen(filename, "rb");
    if(!f)
    {
        dprintf("Failed to open %s", filename);
        return false;
    }
    uint32_t fileLen;
    fseek(f, 0, SEEK_END);
    fileLen= ftell(f);
    fseek(f, 0, SEEK_SET);

    // read.
    char* buf = new char[fileLen+1];
    memset(buf, 0, fileLen+1);
    char* cur = buf;
    size_t numRead = 0;
    uint32_t remain = fileLen;
    while(remain)
    {
        numRead = fread(cur, 1, remain, f);
        if(numRead < 1)
            return false;
        remain-=(uint32_t)numRead;
        cur+=numRead;
    }
    fclose(f);

    SpawnEvent spawnEvent;
    bool ret = true;
    bool header = true;
    cur = buf;
    char* s = buf;
    char* e;
    int stage = 0;
    while(s-buf < fileLen)
    {
        e = strchr(s, '\n');
        if(!e)
            e = buf + fileLen;
        *e = NULL;

        switch(stage)
        {
        case 0: // header.
            break;
        case 1:
            ret = parseMapName(s, mapName);
            break;
        case 2: // header.
            break;
        case 3:
            ret = parseSpawnEvent(s, &spawnEvent);
            if(ret)
            {
                gWorld->getEnemyManager()->addSpawnEvent(spawnEvent);
                dumpSpawnEvent(&spawnEvent);
            }
            else
                delete[] buf;
                return true;
            break;
        default:
            ret = false;
            dprintf("Unknown data found");
        }

        ++stage;
        
        if(ret == false)
        {
            dprintf("Error parsing at stage %d", stage-1);
            assert(0);
        }

        s = e+1;
    }

    delete[] buf;
    return false;
}

} // namespace pammo
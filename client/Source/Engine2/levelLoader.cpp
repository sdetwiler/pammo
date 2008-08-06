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
    uint32_t numRead = 0;
    uint32_t remain = fileLen;
    while(remain)
    {
        numRead = fread(cur, 1, remain, f);
        if(numRead == 0)
            return false;
        remain-=numRead;
        cur+=numRead;
    }
    fclose(f);

    cur = buf;
    char* s = buf;
    char* e;
    while(s-buf < fileLen)
    {
        char* e = strchr(s, '\n');
        if(!e)
            e = buf + fileLen;

        // do the parse of the current line.
        // startTime, duration, count, enemyType, spawnId
        SpawnEvent spawnEvent;

        // If the line begins with a digit, process. Otherwise assume it's the field header.
        if(isdigit(*s))
        {
            int numItems = sscanf(s, "%lld,%lld,%d,%d,%d,\"%s\"", &spawnEvent.mStartTime, &spawnEvent.mDuration, &spawnEvent.mCount, &spawnEvent.mEnemyType, &spawnEvent.mSpawnId, mapName);
            // sscanf includes the trailing " on strings even if it's in the format string. Strip it.
            if(numItems==6)
            {
                mapName[strlen(mapName)-1] = 0;
            }
            // Convert to uSec
            spawnEvent.mDuration*=1000000;
            spawnEvent.mStartTime*=1000000;

            dprintf("SpawnEvent:\n%lld\n%lld\n%d\n%d\n%d\n%s\n", spawnEvent.mStartTime, spawnEvent.mDuration, spawnEvent.mCount, spawnEvent.mEnemyType, spawnEvent.mSpawnId, mapName);
            gWorld->getEnemyManager()->addSpawnEvent(spawnEvent);
        }
        else
        {
            dprintf("Skipping line \"%s\"", s);
        }
        s = e+1;
    }

    delete[] buf;
    return true;
}

} // namespace pammo
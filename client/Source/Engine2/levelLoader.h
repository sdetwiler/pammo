#ifndef __LEVELLOADER_H__
#define __LEVELLOADER_H__

#include "pammo.h"
#include "enemyManager.h"

namespace pammo
{

class LevelLoader
{
public:
    LevelLoader();
    virtual ~LevelLoader();

    bool load(char const* filename, char* mapName);

protected:

private:

};


} // namespace pammo

#endif // __LEVELLOADER_H__
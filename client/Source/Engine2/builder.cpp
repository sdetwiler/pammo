#include "builder.h"

#include "world.h"
#include "imageEntity.h"
#include "imageLibrary.h"
#include "map.h"
#include "enemyManager.h"
#include "player.h"
#include "physics.h"
#include "minimap.h"

namespace pammo
{
    
uint16_t readUInt16(char** cur, size_t* remain)
{
    uint16_t tmp;
    assert(*remain >= sizeof(tmp));
    memcpy(&tmp, *cur, sizeof(tmp));
    *cur += sizeof(tmp);
    *remain -= sizeof(tmp);
    return ntohs(tmp);
}
    
float readFloat(char** cur, size_t* remain)
{
    float tmp;
    assert(*remain >= sizeof(tmp));
    memcpy(&tmp, *cur, sizeof(tmp));
    *cur += sizeof(tmp);
    *remain -= sizeof(tmp);
    
    uint32_t* ptr = (uint32_t*)&tmp;
    *ptr = ntohl(*ptr);
    return tmp;
}
    
char* readString(char** cur, size_t* remain)
{
    char* tmp = *cur;
    size_t len = 0;
    while((*cur)[len] != 0)
    {
        ++len;
        assert(*remain >= len + 1);
    }
    assert(*remain >= len + 1);
    *cur += len+1;
    *remain -= len+1;
    return tmp;
}
	
void buildTileMap(World* world, char const* name)
{
    string fullName = string("data/maps/") + name + ".vmap";
	FILE* f = fopen(fullName.c_str(), "rb");
	if(!f)
	{
		dprintf("Error opening map %s / %s", name, fullName.c_str());
		return;
	}
    
    // Read the entire map into memory.
    fseek(f, 0, SEEK_END);
    size_t remain = ftell(f);
    rewind(f);
    char* buffer = new char[remain];
    fread(buffer, remain, 1, f);
    fclose(f);
    char* cur = buffer;
    
    // Get a pointer to the tileMap to fill in as we go... in that style.
    Map* map = world->getMap();
    
    // Verify the map header.
    assert(cur[0] == 'P' && cur[1] == 'I' && cur[2] == 'V' && cur[3] == 1);
    cur += 4;
    remain -= 4;
    
	// Read num materials.
	uint16_t numMaterials = readUInt16(&cur, &remain);
    map->setNumMaterials(numMaterials);
	dprintf("Materials: %d", numMaterials);
	
    // Load each material.
	for(uint32_t i=0; i < numMaterials; ++i)
	{
        char* materialName = readString(&cur, &remain);
        string materialPath = string("data/materials/") + materialName + ".png";
        map->setMaterial(i, materialPath.c_str());
        dprintf("%d - %s", i, materialPath.c_str());
	}
    
    // Load each tile.
    uint16_t tilesX = readUInt16(&cur, &remain);
    uint16_t tilesY = readUInt16(&cur, &remain);
    map->setNumTiles(tilesX, tilesY);
    for(uint16_t y=0; y < tilesY; ++y)
    {
        for(uint16_t x=0; x < tilesX; ++x)
        {
            uint16_t i = readUInt16(&cur, &remain);
            map->setTile(x, y, i);
        }
    }
    
	// Read num props.
	uint16_t numProps = readUInt16(&cur, &remain);
	dprintf("Props: %d", numProps);
    Image** propLookup = new Image*[numProps];

    // Load each prop.
	for(uint32_t i=0; i < numProps; ++i)
	{
        char* propName = readString(&cur, &remain);
        string propPath = string("data/props/") + propName + ".png";
        propLookup[i] = gImageLibrary->reference(propPath.c_str());
        dprintf("%d - %s", i, propPath.c_str());
	}
    
    // Load each entity.
    uint16_t numEntities = readUInt16(&cur, &remain);
    for(uint16_t tmp=0; tmp < numEntities; ++tmp)
    {
        uint16_t i = readUInt16(&cur, &remain);
        dprintf("Prop using %d", i);
        float posX = readFloat(&cur, &remain);
        float posY = readFloat(&cur, &remain);
        float scale = readFloat(&cur, &remain);
        float rot = readFloat(&cur, &remain);
        
        if(i>numProps)
        {
            dprintf("Error in map format: prop %d is out of bounds. %d is max %d.\n", i, numProps, htons(i));
            continue;
        }

        if(propLookup[i])
        {
            ImageEntity* e = new ImageEntity(propLookup[i]);
            e->mCenter = Vector2(posX, posY);
            e->mSize *= scale;
            e->mRotation = rot;
            map->addProp(e);
        }
    }
    
    // Cleanup.
    delete[] buffer;
}

void buildCollisionMap(World* world, char const* mapName)
{
    string fullName = string("data/maps/") + mapName + ".omap";
	FILE* f = fopen(fullName.c_str(), "rb");
	if(!f)
	{
		dprintf("Error opening collision map %s / %s", mapName, fullName.c_str());
		return;
	}
    
    // Read the entire collision map into memory.
    fseek(f, 0, SEEK_END);
    size_t remain = ftell(f);
    rewind(f);
    char* buffer = new char[remain];
    fread(buffer, remain, 1, f);
    fclose(f);
    char* cur = buffer;
    
    // Get a pointer to the collision map.
    Physics* physics = gWorld->getPhysics();
    Map* map = gWorld->getMap();
    physics->setMapSize(Vector2(map->getSizeX()*map->getSizeMaterial(), map->getSizeY()*map->getSizeMaterial()));
    gWorld->getMinimap()->setMapSize(Vector2(map->getSizeX()*map->getSizeMaterial(), map->getSizeY()*map->getSizeMaterial()));
    
    // Verify the map header.
    assert(cur[0] == 'P' && cur[1] == 'I' && cur[2] == 'O' && cur[3] == 1);
    cur += 4;
    remain -= 4;
    
    // Read num shapes.
    uint16_t numShapes = readUInt16(&cur, &remain);
    dprintf("Collision Shapes: %d", numShapes);
    
    // Read each shape.
    for(uint32_t s=0; s < numShapes; ++s)
    {
		uint16_t properties = readUInt16(&cur, &remain);
		if(properties == 0) properties = kBarrierCollisionProperties;
        uint16_t numPoints = readUInt16(&cur, &remain);
        dprintf(" Points: %d", numPoints);
        Vector2* points = new Vector2[numPoints];
        for(uint32_t i=0; i < numPoints; ++i)
        {
            points[i].x = readFloat(&cur, &remain);
            points[i].y = readFloat(&cur, &remain);
            dprintf("  (%f, %f)", points[i].x, points[i].y);
        }
        
        physics->addShape(properties, numPoints, points);
    }
    
    // Read num POIs.
    uint16_t numPOIs = readUInt16(&cur, &remain);
    dprintf("POIs: %d", numPOIs);
    
    uint32_t spawnPoints = 0;
    
    // Read each poi.
    for(uint32_t poi=0; poi<numPOIs; ++poi)
    {
        uint16_t properties = readUInt16(&cur, &remain);
        float x = readFloat(&cur, &remain);
        float y = readFloat(&cur, &remain);
        
        if(properties == 0)
        {
            ++spawnPoints;
            dprintf("Spawn Point: %f, %f", x, y);
            world->getEnemyManager()->addSpawnPoint(Vector2(x, y));
        }
        else if(properties == 1)
        {
            world->getPlayer()->setCenter(Vector2(x, y));
        }
    }
    
    // Add a default POI if non are defined in the file.
    if(spawnPoints == 0)
        world->getEnemyManager()->addSpawnPoint(Vector2(0, 0));
    
    // Free the buffer;
    delete[] buffer;
}
    
void buildFromMap(World* world, char const* mapName)
{
    buildTileMap(world, mapName);
    buildCollisionMap(world, mapName);
}

}

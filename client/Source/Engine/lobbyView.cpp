/*
 *  lobbyView.cpp
 *  pammo
 *
 *  Created by James Marr on 6/3/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "lobbyView.h"
#include "world.h"
#include "image.h"
#include "imageLibrary.h"
#include "flameTankVehicle.h"
#include "vehicleSelectView.h"

namespace pammo
{

LobbyView::LobbyView()
    : View()
{
    gGame->queueInitable(this);
    
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/LobbyBackground.png");
    
    // Open bmap directory.
    DIR* dir;
    dir = opendir("data/maps/");
    if(!dir)
    {
        dprintf("Failed to open data/maps/");
        assert(0);
    }
    
    // Load maps.
    float border = 12;
    Vector2 pos(border, 335);
    struct dirent* item;
    while((item = readdir(dir)) != NULL)
    {
        int len = strlen(item->d_name);
        if(strcmp(&(item->d_name[len-5]), ".vmap")) continue;
        
        Room* room = new Room;
        room->mMapName = string(item->d_name).substr(0, len-5);
        room->mImage = gImageLibrary->reference("data/interface/LobbyRoom.png");
        room->mSize = room->mImage->mSize;
        room->mCenter = pos + room->mSize / 2;
        pos[0] += room->mSize[0] + border;
        
        if(pos[0] + room->mSize[0] + border >= getFrameSize()[0])
        {
            pos[0] = border;
            pos[1] += room->mSize[1] + border;
        }
        
        mRooms.push_back(room);
    }
    
    closedir(dir);
}

LobbyView::~LobbyView()
{
    gGame->unregisterTouchable(this);
    gGame->unregisterDrawable(this);
    gGame->unregisterUpdateable(this);
    
    gImageLibrary->unreference(mBackground);
    
    for(RoomVector::iterator i=mRooms.begin(); i !=mRooms.end(); ++i)
    {
        gImageLibrary->unreference((*i)->mImage);
        delete *i;
    }
}

void LobbyView::init()
{
    gGame->registerTouchable(this);
    gGame->registerDrawable(this);
    gGame->registerUpdateable(this);
}

void LobbyView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    drawImage(mBackground, trans, 1);
    
    for(RoomVector::iterator i=mRooms.begin(); i != mRooms.end(); ++i)
    {
        Room* room = *i;
        trans = Transform2::createTranslation(room->mCenter - room->mSize/2) *
                Transform2::createScale(room->mSize);
        drawImage(room->mImage, trans, 1);
    }
}

bool LobbyView::touch(uint32_t count, Touch* touches)
{
    // No multitouch, and only on release.
    if(count != 1) return false;
    if(touches[0].mPhase != Touch::PhaseEnd) return false;
    
    Vector2 pos = touches[0].mLocation;
    
    // Figure out if you touched anything.
    for(RoomVector::iterator i=mRooms.begin(); i != mRooms.end(); ++i)
    {
        Room* room = *i;
        Vector2 ul = room->mCenter - room->mSize/2;
        Vector2 lr = room->mCenter + room->mSize/2;
        
        // Is this touch inside of this room?
        if(pos.x < ul.x || pos.y < ul.y
            || pos.x > lr.x || pos.y > lr.y) continue;
            
        dprintf("Selected %s", room->mMapName.c_str());
        gotoWorld(room->mMapName.c_str());
        return true;
    }
    return false;
}

void LobbyView::update()
{}

void LobbyView::gotoWorld(char const* mapName)
{
    // Now create the world.
    //new World(mapName, VEHICLE_FLAMETANK);
    new VehicleSelectView(mapName);
    
    // Delete ourselves.
    gGame->queueDeleteable(this);
}

}


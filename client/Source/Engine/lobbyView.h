/*
 *  lobbyView.h
 *  pammo
 *
 *  Created by James Marr on 6/3/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __LOBBY_VIEW_H__
#define __LOBBY_VIEW_H__

#include "pammo.h"
#include "widget.h"

namespace pammo
{

struct Image;

class LobbyView : public View
{
public:
    LobbyView();
    virtual ~LobbyView();
    
    virtual void init();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    
private:
    void gotoWorld(char const* mapName);
    
    Image* mBackground;
    
    struct Room
    {
        string mMapName;
        Vector2 mCenter;
        Vector2 mSize;
        Image* mImage;
    };
    typedef vector< Room* > RoomVector;
    RoomVector mRooms;
};
    
}

#endif
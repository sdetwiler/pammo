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

class LobbyView : public Widget
{
public:
    LobbyView();
    ~LobbyView();
    
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update(int delta);
    
private:
    void createWorld();
};
    
}

#endif
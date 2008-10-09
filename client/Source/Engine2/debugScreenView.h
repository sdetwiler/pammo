#ifndef __DEBUGSCREEN_VIEW_H__
#define __DEBUGSCREEN_VIEW_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

struct Image;

class DebugScreenView : public View
{
public:
    DebugScreenView();
    virtual ~DebugScreenView();
    
    virtual void update();
    virtual void draw();
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getUpdatePriority() const;

private:
};
  
}

#endif
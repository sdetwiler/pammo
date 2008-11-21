#ifndef __DEATHCARD_VIEW_H__
#define __DEATHCARD_VIEW_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

struct Image;

class DeathCardView : public View
{
public:
    DeathCardView();
    virtual ~DeathCardView();
    
    virtual void update();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    
    bool isTouched() const;
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getUpdatePriority() const;

private:
    bool mTouched;
    Image* mBackground;
    float mAlpha;
};
  
}

#endif
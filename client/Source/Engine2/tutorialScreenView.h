#ifndef __TUTORIALSCREEN_VIEW_H__
#define __TUTORIALSCREEN_VIEW_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

struct Image;

class TutorialScreenView : public View
{
public:
    TutorialScreenView();
    virtual ~TutorialScreenView();
    
    virtual void update();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getUpdatePriority() const;

private:
    
    Image* mBackground;
};
  
}

#endif
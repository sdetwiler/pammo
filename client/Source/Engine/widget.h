#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "game.h"

namespace pammo
{

class Widget :
    public Drawable,
    public Touchable,
    public Updateable
{
public:
    Widget();
    virtual ~Widget();
    virtual void draw() = 0;
    virtual bool touch(uint32_t count, Touch* touches) = 0;
    virtual void update(int delta) = 0;

    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;

    virtual void setTouchPriority(uint32_t prio);
    virtual void setDrawPriority(uint32_t prio); 

protected:
private:
    uint32_t mDrawPriority;
    uint32_t mTouchPriority;
};

} // namespace pammo

#endif // __WIDGET_H__
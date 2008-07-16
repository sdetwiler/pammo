#include "widget.h"

Widget::Widget()
    : Drawable(),
    Touchable(),
    Updateable()
{
    mTouchPriority = 100;
    mDrawPriority = 100;
}

Widget::~Widget()
{
}

void Widget::setDrawPriority(uint32_t prio)
{
    mDrawPriority = prio;
}

void Widget::setTouchPriority(uint32_t prio)
{
    mTouchPriority = prio;
}

uint32_t Widget::getDrawPriority() const
{
    return mDrawPriority;
}

uint32_t Widget::getTouchPriority() const
{
    return mTouchPriority;
}
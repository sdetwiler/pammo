#ifndef __INTERFACEVIEW_H__
#define __INTERFACEVIEW_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

struct Image;

class InterfaceView : public View
{
public:
    InterfaceView();
    virtual ~InterfaceView();

    virtual uint32_t getDrawPriority() const;
    virtual void draw();

private:
    Image* mImage;
};

} // namespace pammo

#endif // __INTERFACEVIEW_H__
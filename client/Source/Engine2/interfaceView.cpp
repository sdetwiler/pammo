#include "interfaceView.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

InterfaceView::InterfaceView()
{
    mImage = gImageLibrary->getImage(INTERFACE_INGAME_OVERLAY);
}

InterfaceView::~InterfaceView()
{
    gImageLibrary->purgeImage(mImage);
}

uint32_t InterfaceView::getDrawPriority() const
{
    return kInterfaceViewPriority;
}

void InterfaceView::draw()
{
    Transform2 trans = Transform2::createScale(mImage->mSize);
    drawImage(mImage, trans, 1.0f);
}




} // namespace pammo
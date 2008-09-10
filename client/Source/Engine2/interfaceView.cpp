#include "interfaceView.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

InterfaceView::InterfaceView()
{
    mImage = gImageLibrary->reference("data/interface/vignette.png");
}

InterfaceView::~InterfaceView()
{
    gImageLibrary->unreference(mImage);
}

uint32_t InterfaceView::getDrawPriority() const
{
    return kInterfaceViewPriority;
}

void InterfaceView::draw()
{
    Transform2 trans = Transform2::createTranslation(Vector2((480-512)/2, (320-512)/2));
    trans*= Transform2::createScale(mImage->mSize);
        
    drawImage(mImage, trans, 1.0f);
}




} // namespace pammo
#include "interfaceView.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

InterfaceView::InterfaceView()
{
// SCD: THIS IS INCORRECT BUT NOT IN USE. CHANGE WHEN WE HAVE A REAL IMAGE.
    mImage = gImageLibrary->getImage(INTERFACE_BACKGROUND_MAIN);
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
    Transform2 trans = Transform2::createTranslation(Vector2((480-512)/2, (320-512)/2));
    trans*= Transform2::createScale(mImage->mSize);
        
    drawImage(mImage, trans, 1.0f);
}




} // namespace pammo
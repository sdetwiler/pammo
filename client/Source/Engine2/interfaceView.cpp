#include "interfaceView.h"

#include "drawImage.h"
#include "imageLibrary.h"
#include "imageEntity.h"

namespace pammo
{

InterfaceView::InterfaceView()
{
    mImage = gImageLibrary->getImage(INTERFACE_OVERLAY);
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
    const GLfloat upperVerts[] = {
    0.0, 0.0,
    1.0, 0.0,
    0.0, 0.1,
    1.0, 0.1,
    };

    const GLfloat lowerVerts[] = {
    0.0, 0.9,
    1.0, 0.9,
    0.0, 1.0,
    1.0, 1.0,
    };

    const GLfloat leftVerts[] = {
    0.0, 0.0,
    0.1, 0.0,
    0.0, 1.0,
    0.1, 1.0,
    };

    const GLfloat rightVerts[] = {
    0.9, 0.0,
    1.0, 0.0,
    0.9, 1.0,
    1.0, 1.0,
    };

    float trans[16];
    memset(trans, 0, sizeof(trans));
    trans[0] = mImage->mSize.x;
    trans[5] = mImage->mSize.y;
    trans[10] = 1.0;
    trans[15] = 1.0;

    glLoadMatrixf(trans);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0);
    glBindTexture(GL_TEXTURE_2D, mImage->mTexture);
    
    Vector2 texCoords[4];
    
    // Draw upper poly.
    memcpy(texCoords, (float*)mImage->mTexCoords, sizeof(texCoords));
    texCoords[2].y = texCoords[3].y = upperVerts[5]*(1 - texCoords[1].y) + texCoords[1].y;
    glVertexPointer(2, GL_FLOAT, 0, upperVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw lower poly.
    memcpy(texCoords, (float*)mImage->mTexCoords, sizeof(texCoords));
    texCoords[0].y = texCoords[1].y = 1 - (1 - lowerVerts[1])*(1 - texCoords[1].y);
    glVertexPointer(2, GL_FLOAT, 0, lowerVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw left poly.
    memcpy(texCoords, (float*)mImage->mTexCoords, sizeof(texCoords));
    texCoords[1].x = texCoords[3].x = leftVerts[2] * texCoords[1].x;
    glVertexPointer(2, GL_FLOAT, 0, leftVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw right poly.
    memcpy(texCoords, (float*)mImage->mTexCoords, sizeof(texCoords));
    texCoords[0].x = texCoords[2].x = rightVerts[0] * texCoords[1].x;
    glVertexPointer(2, GL_FLOAT, 0, rightVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glLoadIdentity();
    ImageEntity::resetTextureCache();
}

} // namespace pammo
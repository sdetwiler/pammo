#include "drawImage.h"
#include "imageEntity.h"

namespace pammo
{

const GLfloat spriteVertices[] = {
0.0, 0.0,
1.0, 0.0,
0.0, 1.0,
1.0, 1.0,
};

void drawImage(Image* image, Transform2 const& transform, float alpha)
{
    float trans[16];
    memset(trans, 0, sizeof(trans));
    trans[0] = transform[0];
    trans[1] = transform[1];
    trans[4] = transform[2];
    trans[5] = transform[3];
    trans[10] = 1.0;
    trans[12] = transform[4];
    trans[13] = transform[5];
    trans[15] = 1.0;

    glLoadMatrixf(trans);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBindTexture(GL_TEXTURE_2D, image->mTexture);
    
    glVertexPointer(2, GL_FLOAT, 0, spriteVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)image->mTexCoords);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glLoadIdentity();
    ImageEntity::resetTextureCache();
}

}
#include "image.h"

const GLfloat spriteVertices[] = {
0, 0,
1, 0,
0, 1,
1, 1,
};

// Sets up an array of values for the texture coordinates.
const GLshort spriteTexcoords[] = {
0, 0,
1, 0,
0, 1,
1, 1,
};

void drawImage(Image* image, Transform2 const& transform)
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
	
	glVertexPointer(2, GL_FLOAT, 0, spriteVertices);
	glTexCoordPointer(2, GL_SHORT, 0, spriteTexcoords);

    glLoadMatrixf(trans);

    glBindTexture(GL_TEXTURE_2D, image->mTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

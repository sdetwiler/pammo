#include "image.h"

void drawImage(Image* image, Transform2 const& transform)
{
    glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
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

    glMultMatrixf(trans);


    glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f,  0.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 0.0f,  0.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,  1.0f,  0.0f);	// Top Left Of The Texture and Quad
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);			// Enable Texture Mapping
}

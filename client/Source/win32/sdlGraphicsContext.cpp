#include "sdlGraphicsContext.h"
#include <SDL_image.h>
#include <gl/glu.h>

const uint32_t SdlGraphicsContext::ScreenWidth=320;
const uint32_t SdlGraphicsContext::ScreenHeight=480;

Vector2 SdlImage::getSize()
{
    if(!mSurface)
        return Vector2(0,0);
    return Vector2((float)mSurface->w, (float)mSurface->h);
}

SdlGraphicsContext::SdlGraphicsContext()
{
    SDL_Init(SDL_INIT_VIDEO);
    mScreen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 0, SDL_OPENGL);
    SDL_WM_SetCaption("PAMMO", NULL);

    glViewport(0, 0, ScreenWidth, ScreenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, ScreenWidth, ScreenHeight, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

SdlGraphicsContext::~SdlGraphicsContext()
{
}

Vector2 SdlGraphicsContext::getFrameSize()
{
    return Vector2((float)ScreenWidth, (float)ScreenHeight);
}

void SdlGraphicsContext::frameBegin()
{
    glClear(GL_COLOR_BUFFER_BIT);		// Clear The Screen
}

void SdlGraphicsContext::frameEnd()
{
    SDL_GL_SwapBuffers();
}

void SdlGraphicsContext::setCameraTransform(Transform2 const& transform)
{
    glLoadIdentity();
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
}

Image* SdlGraphicsContext::openImage(char const* path)
{
    SdlImage* img = new SdlImage;
    img->mSurface = IMG_Load(path);
    if(!img->mSurface)
    {
        dprintf("Failed to load %s\n", path);
        delete img;
        return NULL;
    }

    glGenTextures(1, &img->mTexture);
    glBindTexture(GL_TEXTURE_2D, img->mTexture);
     
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); // scale linearly when image smalled than texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->mSurface->w, img->mSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->mSurface->pixels);

    return img;
}

void SdlGraphicsContext::closeImage(Image* image)
{
    glDeleteTextures(1, &((SdlImage*)image)->mTexture);
    SDL_FreeSurface(((SdlImage*)image)->mSurface);

    delete image;
}

void SdlGraphicsContext::drawImage(Image* image, Transform2 const& transform, Transform2 const& invTransform)
{
    SdlImage* sdlImage = (SdlImage*)image;

    glPushMatrix();
    //glLoadIdentity();
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

    
//    glTranslatef(ScreenWidth/2.0, ScreenHeight/2.0, 0);
    //glScalef(50.0, 50.0, 1.0);

    


    glBindTexture(GL_TEXTURE_2D, sdlImage->mTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f,  0.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 0.0f,  0.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,  1.0f,  0.0f);	// Top Left Of The Texture and Quad
    glEnd();
    glPopMatrix();
}

SDL_Surface* SdlGraphicsContext::getScreen()
{
    return mScreen;
}
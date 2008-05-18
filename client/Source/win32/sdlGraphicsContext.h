#ifndef __SDLGRAPHICSCONTEXT_H__
#define __SDLGRAPHICSCONTEXT_H__

#include "pammo.h"
#include "graphicsContext.h"
#include <windows.h>
#include <gl/gl.h>
#include <SDL.h>

class SdlImage :
    public Image
{
public:
	virtual Vector2 getSize();
	Vector2 mSize;
    SDL_Surface* mSurface;
    GLuint mTexture;

protected:
private:
};

class SdlGraphicsContext : 
    public GraphicsContext
{
public:
    SdlGraphicsContext();
    virtual ~SdlGraphicsContext();

    virtual Vector2 getFrameSize();
	
	virtual void frameBegin();
	virtual void frameEnd();
	
	virtual void setCameraTransform(Transform2 const& transform);
	
	virtual Image* openImage(char const* path);
	virtual void closeImage(Image* image);
	virtual void drawImage(Image* image, Transform2 const& transform, Transform2 const& invTransform);

    static const uint32_t ScreenWidth;
    static const uint32_t ScreenHeight;

    SDL_Surface* getScreen();
private:
    SDL_Surface* mScreen;
	
};


#endif // __SDLGRAPHICSCONTEXT_H__
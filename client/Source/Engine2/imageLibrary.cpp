#include "imageLibrary.h"
#include "view.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif

namespace pammo
{

ImageTexture gImagePath[] = 
{
    {"interface/iconWeaponHighlight.png", NULL, 0},         // 0
    {"interface/iconWeaponLightningGun.png", NULL, 0},      // 1 
    {"interface/iconWeaponGoo.png", NULL, 0},               // 2
    {"interface/iconWeaponGrenadeLauncher.png", NULL, 0},   // 3
    {"interface/iconShieldEnable.png", NULL, 0},           // 4
    {"interface/iconShieldDisable.png", NULL, 0},           // 5

    {"interface/backgroundMain.png", NULL, 0},        // 6
    {"interface/backgroundTutorial.png", NULL, 0},          // 7
    {"interface/backgroundCredits.png", NULL, 0},           // 8

    {"interface/shieldMapLifeBar.png", NULL, 0},            // 9
    {"interface/healthDot.png", NULL, 0},                   // 10

    {"interface/ringMovement.png", NULL, 0},                // 11
    {"interface/ringTarget.png", NULL, 0},                  // 12

    {"interface/deathCards/00.png", NULL, 0},               // 13
    {"interface/deathCards/01.png", NULL, 0},               // 14
    {"interface/deathCards/02.png", NULL, 0},               // 15
    {"interface/deathCards/03.png", NULL, 0},               // 16
    {"interface/deathCards/04.png", NULL, 0},               // 17
    {"interface/deathCards/05.png", NULL, 0},               // 18
    {"interface/deathCards/06.png", NULL, 0},               // 19
    {"interface/deathCards/07.png", NULL, 0},               // 20
    {"interface/deathCards/08.png", NULL, 0},               // 21
    {"interface/deathCards/09.png", NULL, 0},               // 22
    {"interface/deathCards/10.png", NULL, 0},               // 23

    {"vehicles/tank/00.png", NULL, 0},                      // 24
    {"vehicles/tank/01.png", NULL, 0},                      // 25
    {"vehicles/tank/02.png", NULL, 0},                      // 26
    {"vehicles/tank/turret/00.png", NULL, 0},               // 27

    {"fonts/caslonAntique/0.png", NULL, 0},                 // 28
    {"fonts/caslonAntique/1.png", NULL, 0},                 // 29
    {"fonts/caslonAntique/2.png", NULL, 0},                 // 30
    {"fonts/caslonAntique/3.png", NULL, 0},                 // 31
    {"fonts/caslonAntique/4.png", NULL, 0},                 // 32
    {"fonts/caslonAntique/5.png", NULL, 0},                 // 33
    {"fonts/caslonAntique/6.png", NULL, 0},                 // 34
    {"fonts/caslonAntique/7.png", NULL, 0},                 // 35
    {"fonts/caslonAntique/8.png", NULL, 0},                 // 36
    {"fonts/caslonAntique/9.png", NULL, 0},                 // 37


    {"particles/flame/00.png", NULL, 0},                    // 38

    {"particles/heatSeaker/00.png", NULL, 0},               // 39
    {"particles/heatSeaker/tail/00.png", NULL, 0},          // 40

    {"particles/mine.png", NULL, 0},                        // 41

    {"particles/ball.png", NULL, 0},                        // 42
    {"particles/shadow/00.png", NULL, 0},                      // 43

    {"particles/machineGun/00.png", NULL, 0},               // 44
    {"particles/machineGun/01.png", NULL, 0},               // 45
    {"particles/machineGun/02.png", NULL, 0},               // 46

    {"particles/goo/00.png", NULL, 0},                      // 47
    {"particles/goo/01.png", NULL, 0},                      // 48
    {"particles/goo/02.png", NULL, 0},                      // 49

    {"particles/smoke/00.png", NULL, 0},                    // 50

    {"particles/dust/00.png", NULL, 0},                     // 51

    {"particles/rubble/00.png", NULL, 0},                   // 52

    {"particles/lightningGlow/00.png", NULL, 0},            // 53

    {"particles/lightning/00.png", NULL, 0},                // 54
    {"particles/lightning/01.png", NULL, 0},                // 55
    {"particles/lightning/02.png", NULL, 0},                // 56

    {"particles/grenade/00.png", NULL, 0},                  // 57

    {"particles/explosion/00.png", NULL, 0},                // 58
    {"particles/explosion/01.png", NULL, 0},                // 59

    {"particles/shield/00.png", NULL, 0},                   // 60
    {"particles/shield/01.png", NULL, 0},                   // 61
    {"particles/shield/02.png", NULL, 0},                   // 62

    {"particles/fakeWeapon.png", NULL, 0},                  // 63

    {"vehicles/bigBoy/00.png", NULL, 0},                    // 64

    {"vehicles/flameTank/00.png", NULL, 0},                 // 65
    {"vehicles/flameTank/01.png", NULL, 0},                 // 66
    {"vehicles/flameTank/02.png", NULL, 0},                 // 67
    {"vehicles/flameTank/03.png", NULL, 0},                 // 68
    {"vehicles/flameTank/04.png", NULL, 0},                 // 69
    {"vehicles/flameTank/turret/00.png", NULL, 0},          // 70

    {"vehicles/killerTop/00.png", NULL, 0},                 // 71

    {"vehicles/mineLayer/00.png", NULL, 0},                 // 72

    {"vehicles/torpedo/00.png", NULL, 0},                   // 73

    {"vehicles/trebuchet/00.png", NULL, 0},                 // 74

    {"particles/jetFlame/00.png", NULL, 0},                 // 75

    {"interface/buttonMask.png", NULL, 0},                  // 76


};


Image* makeSubImage(RawImage* raw, Vector2 start, Vector2 size)
{
	Image* image = new Image();
    image->mSize = size;
    
    // Setup pixel store.
    uint32_t srcRow = (uint32_t)raw->mSize.x*raw->mBytesPerPixel;
    uint32_t dstRow = (uint32_t)size.x*raw->mBytesPerPixel;
    uint8_t* pixels = new uint8_t[(uint32_t)(dstRow*size.y)];
    for(uint32_t y=0; y < size.y; ++y)
    {
        uint32_t srcOffset = y*dstRow;
        uint32_t dstOffset = (uint32_t)start.x*raw->mBytesPerPixel + (start.y+y)*srcRow;
        uint8_t* src = pixels + srcOffset;
        uint8_t* dst = raw->mPixels + dstOffset;
        memcpy(src, dst, dstRow);
    }

    int mode;
    if(raw->mBytesPerPixel == 3)
        mode = GL_RGB;
    else if(raw->mBytesPerPixel == 4)
        mode = GL_RGBA;
    
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &image->mTexture);
	glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	glTexImage2D(GL_TEXTURE_2D, 0, mode, size.x, size.y, 0, mode, GL_UNSIGNED_BYTE, pixels);
    
    // Restore pixel store.
    delete[] pixels;
    
    return image;
}

Image* openImage(char const* path)
{
    RawImage raw;
    openRawImage(path, &raw);
    
	Image* image = new Image();
    image->mSize = raw.mSize;
    
	int mode;
    if(raw.mBytesPerPixel == 3)
        mode = GL_RGB;
    else if(raw.mBytesPerPixel == 4)
        mode = GL_RGBA;
	
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &image->mTexture);
	glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->mSize.x, image->mSize.y, 0, mode, GL_UNSIGNED_BYTE, raw.mPixels);
	
	delete[] raw.mPixels;
	return image;
}
    
ImageLibrary* gImageLibrary = NULL;

ImageLibrary::ImageLibrary()
{
    mCurrImageLoad = 0;
    mObserver = 0;
    mNumImages = sizeof(gImagePath)/sizeof(ImageTexture);
}

ImageLibrary::~ImageLibrary()
{
    for(StringImageRefMap::iterator i=mImages.begin(); i != mImages.end(); ++i)
    {
        glDeleteTextures(1, &i->second->mImage->mTexture);
        delete i->second->mImage;
        delete i->second;
    }
}

void ImageLibrary::setObserver(ImageLibraryObserver* o)
{
    mObserver = o;

    if(mObserver)
    {
        mObserver->onPercentLoaded((float)mCurrImageLoad/(float)mNumImages);
    }
}



uint32_t ImageLibrary::getUpdatePriority() const
{
    return kImageLibraryPriority;
}


void ImageLibrary::load(uint32_t id)
{
    char filename[256];
    snprintf(filename, 256, "data/%s", gImagePath[id].mPath);
    dprintf("ImageLibrary is loading %d (%s)", id, filename);
    if(filename == NULL)
    {
        int* a = NULL;
        *a = 1;
    }

    gImagePath[id].mImage = openImage(filename);
    gImagePath[id].mImage->mTableIndex = id;
}

void ImageLibrary::update()
{
    if(mCurrImageLoad < mNumImages)
    {
        if(gImagePath[mCurrImageLoad].mImage == NULL)
            load(mCurrImageLoad);

        ++mCurrImageLoad;

        if(mObserver)
        {
            mObserver->onPercentLoaded((float)mCurrImageLoad/(float)mNumImages);
        }
    }
}

Image* ImageLibrary::reference(uint32_t id)
{
    if(gImagePath[id].mImage == NULL)
    {
        load(id);
    }

    ++gImagePath[id].mRefCount;
    return gImagePath[id].mImage;
}

void ImageLibrary::reference(Image* image)
{
    ++gImagePath[image->mTableIndex].mRefCount;
}

void ImageLibrary::unreference(Image* image)
{
    --gImagePath[image->mTableIndex].mRefCount;
}


}
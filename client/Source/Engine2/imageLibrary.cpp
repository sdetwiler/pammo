#include "imageLibrary.h"
#include "view.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif

namespace pammo
{

ImageTexture gImagePath[] = 
{
    {"interface/iconWeaponHighlight.png", NULL, true},         // 0
    {"interface/iconWeaponLightningGun.png", NULL, true},      // 1 
    {"interface/iconWeaponGoo.png", NULL, true},               // 2
    {"interface/iconWeaponGrenadeLauncher.png", NULL, true},   // 3
    {"interface/iconShieldEnable.png", NULL, true},           // 4
    {"interface/iconShieldDisable.png", NULL, true},           // 5

    {"../Default.png", NULL, false},        // 6
    {"interface/backgroundTutorial.png", NULL, false},          // 7
    {"interface/backgroundCredits.png", NULL, false},           // 8

    {"interface/shieldMapLifeBar.png", NULL, true},            // 9
    {"interface/healthDot.png", NULL, true},                   // 10

    {"interface/ringMovement.png", NULL, true},                // 11
    {"interface/ringTarget.png", NULL, true},                  // 12

    {"interface/deathCards/00.png", NULL, false},               // 13
    {"interface/deathCards/01.png", NULL, false},               // 14
    {"interface/deathCards/02.png", NULL, false},               // 15
    {"interface/deathCards/03.png", NULL, false},               // 16
    {"interface/deathCards/04.png", NULL, false},               // 17
    {"interface/deathCards/05.png", NULL, false},               // 18
    {"interface/deathCards/06.png", NULL, false},               // 19
    {"interface/deathCards/07.png", NULL, false},               // 20
    {"interface/deathCards/08.png", NULL, false},               // 21
    {"interface/deathCards/09.png", NULL, false},               // 22
    {"interface/deathCards/10.png", NULL, false},               // 23

    {"vehicles/tank/00.png", NULL, true},                      // 24
    {"vehicles/tank/01.png", NULL, true},                      // 25
    {"vehicles/tank/02.png", NULL, true},                      // 26
    {"vehicles/tank/turret/00.png", NULL, true},               // 27

    {"fonts/caslonAntique/0.png", NULL, true},                 // 28
    {"fonts/caslonAntique/1.png", NULL, true},                 // 29
    {"fonts/caslonAntique/2.png", NULL, true},                 // 30
    {"fonts/caslonAntique/3.png", NULL, true},                 // 31
    {"fonts/caslonAntique/4.png", NULL, true},                 // 32
    {"fonts/caslonAntique/5.png", NULL, true},                 // 33
    {"fonts/caslonAntique/6.png", NULL, true},                 // 34
    {"fonts/caslonAntique/7.png", NULL, true},                 // 35
    {"fonts/caslonAntique/8.png", NULL, true},                 // 36
    {"fonts/caslonAntique/9.png", NULL, true},                 // 37


    {"particles/flame/00.png", NULL, true},                    // 38

    {"particles/heatSeaker/00.png", NULL, true},               // 39
    {"particles/heatSeaker/tail/00.png", NULL, true},          // 40

    {"particles/mine.png", NULL, true},                        // 41

    {"particles/ball.png", NULL, true},                        // 42
    {"particles/shadow/00.png", NULL, true},                      // 43

    {"particles/machineGun/00.png", NULL, true},               // 44
    {"particles/machineGun/01.png", NULL, true},               // 45
    {"particles/machineGun/02.png", NULL, true},               // 46

    {"particles/goo/00.png", NULL, true},                      // 47
    {"particles/goo/01.png", NULL, true},                      // 48
    {"particles/goo/02.png", NULL, true},                      // 49

    {"particles/smoke/00.png", NULL, true},                    // 50

    {"particles/dust/00.png", NULL, true},                     // 51

    {"particles/rubble/00.png", NULL, true},                   // 52

    {"particles/lightningGlow/00.png", NULL, true},            // 53

    {"particles/lightning/00.png", NULL, true},                // 54
    {"particles/lightning/01.png", NULL, true},                // 55
    {"particles/lightning/02.png", NULL, true},                // 56

    {"particles/grenade/00.png", NULL, true},                  // 57

    {"particles/explosion/00.png", NULL, true},                // 58
    {"particles/explosion/01.png", NULL, true},                // 59

    {"particles/shield/00.png", NULL, true},                   // 60
    {"particles/shield/01.png", NULL, true},                   // 61
    {"particles/shield/02.png", NULL, true},                   // 62

    {"particles/fakeWeapon.png", NULL, true},                  // 63

    {"vehicles/bigBoy/00.png", NULL, true},                    // 64

    {"vehicles/flameTank/00.png", NULL, true},                 // 65
    {"vehicles/flameTank/01.png", NULL, true},                 // 66
    {"vehicles/flameTank/02.png", NULL, true},                 // 67
    {"vehicles/flameTank/03.png", NULL, true},                 // 68
    {"vehicles/flameTank/04.png", NULL, true},                 // 69
    {"vehicles/flameTank/turret/00.png", NULL, true},          // 70

    {"vehicles/killerTop/00.png", NULL, true},                 // 71

    {"vehicles/mineLayer/00.png", NULL, true},                 // 72

    {"vehicles/torpedo/00.png", NULL, true},                   // 73

    {"vehicles/trebuchet/00.png", NULL, true},                 // 74

    {"particles/jetFlame/00.png", NULL, true},                 // 75

    {"interface/buttonMask.png", NULL, true},                  // 76
    
    {"particles/powerupLifeUpgrade.png", NULL, false},              // 77
    {"particles/powerupLifeRestore.png", NULL, false},              // 78
    {"particles/powerupEnergyUpgrade.png", NULL, false},            // 79
    {"particles/powerupEnergyRestore.png", NULL, false},            // 80
    {"particles/powerupShield.png", NULL, false},                   // 81
    {"particles/powerupGooWeapon.png", NULL, false},                // 82
    {"particles/powerupGrenadeLauncherWeapon.png", NULL, false},    // 83
    
    {"backdroptiles/testmap/preview.png", NULL, false},    // 84
    
    {"backdroptiles/testmap/0-0.png", NULL, false},
    {"backdroptiles/testmap/0-1.png", NULL, false},
    {"backdroptiles/testmap/0-2.png", NULL, false},
    {"backdroptiles/testmap/0-3.png", NULL, false},
    {"backdroptiles/testmap/0-4.png", NULL, false},
    {"backdroptiles/testmap/0-5.png", NULL, false},
    {"backdroptiles/testmap/0-6.png", NULL, false},
    {"backdroptiles/testmap/0-7.png", NULL, false},
    
    {"backdroptiles/testmap/1-0.png", NULL, false},
    {"backdroptiles/testmap/1-1.png", NULL, false},
    {"backdroptiles/testmap/1-2.png", NULL, false},
    {"backdroptiles/testmap/1-3.png", NULL, false},
    {"backdroptiles/testmap/1-4.png", NULL, false},
    {"backdroptiles/testmap/1-5.png", NULL, false},
    {"backdroptiles/testmap/1-6.png", NULL, false},
    {"backdroptiles/testmap/1-7.png", NULL, false},
    
    {"backdroptiles/testmap/2-0.png", NULL, false},
    {"backdroptiles/testmap/2-1.png", NULL, false},
    {"backdroptiles/testmap/2-2.png", NULL, false},
    {"backdroptiles/testmap/2-3.png", NULL, false},
    {"backdroptiles/testmap/2-4.png", NULL, false},
    {"backdroptiles/testmap/2-5.png", NULL, false},
    {"backdroptiles/testmap/2-6.png", NULL, false},
    {"backdroptiles/testmap/2-7.png", NULL, false},
    
    {"backdroptiles/testmap/3-0.png", NULL, false},
    {"backdroptiles/testmap/3-1.png", NULL, false},
    {"backdroptiles/testmap/3-2.png", NULL, false},
    {"backdroptiles/testmap/3-3.png", NULL, false},
    {"backdroptiles/testmap/3-4.png", NULL, false},
    {"backdroptiles/testmap/3-5.png", NULL, false},
    {"backdroptiles/testmap/3-6.png", NULL, false},
    {"backdroptiles/testmap/3-7.png", NULL, false},
    
    {"backdroptiles/testmap/4-0.png", NULL, false},
    {"backdroptiles/testmap/4-1.png", NULL, false},
    {"backdroptiles/testmap/4-2.png", NULL, false},
    {"backdroptiles/testmap/4-3.png", NULL, false},
    {"backdroptiles/testmap/4-4.png", NULL, false},
    {"backdroptiles/testmap/4-5.png", NULL, false},
    {"backdroptiles/testmap/4-6.png", NULL, false},
    {"backdroptiles/testmap/4-7.png", NULL, false},
    
    {"backdroptiles/testmap/5-0.png", NULL, false},
    {"backdroptiles/testmap/5-1.png", NULL, false},
    {"backdroptiles/testmap/5-2.png", NULL, false},
    {"backdroptiles/testmap/5-3.png", NULL, false},
    {"backdroptiles/testmap/5-4.png", NULL, false},
    {"backdroptiles/testmap/5-5.png", NULL, false},
    {"backdroptiles/testmap/5-6.png", NULL, false},
    {"backdroptiles/testmap/5-7.png", NULL, false},
    
    {"backdroptiles/testmap/6-0.png", NULL, false},
    {"backdroptiles/testmap/6-1.png", NULL, false},
    {"backdroptiles/testmap/6-2.png", NULL, false},
    {"backdroptiles/testmap/6-3.png", NULL, false},
    {"backdroptiles/testmap/6-4.png", NULL, false},
    {"backdroptiles/testmap/6-5.png", NULL, false},
    {"backdroptiles/testmap/6-6.png", NULL, false},
    {"backdroptiles/testmap/6-7.png", NULL, false},
    
    {"backdroptiles/testmap/7-0.png", NULL, false},
    {"backdroptiles/testmap/7-1.png", NULL, false},
    {"backdroptiles/testmap/7-2.png", NULL, false},
    {"backdroptiles/testmap/7-3.png", NULL, false},
    {"backdroptiles/testmap/7-4.png", NULL, false},
    {"backdroptiles/testmap/7-5.png", NULL, false},
    {"backdroptiles/testmap/7-6.png", NULL, false},
    {"backdroptiles/testmap/7-7.png", NULL, false},
    
    {"backdroptiles/testmap/8-0.png", NULL, false},
    {"backdroptiles/testmap/8-1.png", NULL, false},
    {"backdroptiles/testmap/8-2.png", NULL, false},
    {"backdroptiles/testmap/8-3.png", NULL, false},
    {"backdroptiles/testmap/8-4.png", NULL, false},
    {"backdroptiles/testmap/8-5.png", NULL, false},
    {"backdroptiles/testmap/8-6.png", NULL, false},
    {"backdroptiles/testmap/8-7.png", NULL, false},
    
    {"backdroptiles/testmap/9-0.png", NULL, false},
    {"backdroptiles/testmap/9-1.png", NULL, false},
    {"backdroptiles/testmap/9-2.png", NULL, false},
    {"backdroptiles/testmap/9-3.png", NULL, false},
    {"backdroptiles/testmap/9-4.png", NULL, false},
    {"backdroptiles/testmap/9-5.png", NULL, false},
    {"backdroptiles/testmap/9-6.png", NULL, false},
    {"backdroptiles/testmap/9-7.png", NULL, false},
    
    {"backdroptiles/testmap/10-0.png", NULL, false},
    {"backdroptiles/testmap/10-1.png", NULL, false},
    {"backdroptiles/testmap/10-2.png", NULL, false},
    {"backdroptiles/testmap/10-3.png", NULL, false},
    {"backdroptiles/testmap/10-4.png", NULL, false},
    {"backdroptiles/testmap/10-5.png", NULL, false},
    {"backdroptiles/testmap/10-6.png", NULL, false},
    {"backdroptiles/testmap/10-7.png", NULL, false},

};

ImageLibrary* gImageLibrary = NULL;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

ImageLibrary::ImageLibrary()
{
    mObserver = 0;
    mImageCount = sizeof(gImagePath)/sizeof(ImageTexture);

    mPreloadIndex = 0;
    mPreloadComplete = false;

    mFree = NULL;
    mToService = NULL;
    mToNotify = NULL;
    // Build the available request queue.
    for(uint32_t i=0; i<kImageLoadStackSize; ++i)
    {
        ImageLoadRequest* curr = new ImageLoadRequest;
        curr->mId = 0;
        curr->mNext = mFree;
        mFree = curr;
    }


    mRunning = true;
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mCondition, NULL);
    pthread_create(&mThread, NULL, threadBootFunc, this);
}

ImageLibrary::~ImageLibrary()
{
    pthread_mutex_destroy(&mMutex);
    pthread_cond_destroy(&mCondition);
}

void ImageLibrary::setObserver(ImageLibraryObserver* o)
{
    mObserver = o;

    if(mObserver)
    {
        if(mPreloadComplete)
            mObserver->onPreloadComplete();
    }
}

uint32_t ImageLibrary::getUpdatePriority() const
{
    return kImageLibraryPriority;
}

Image* ImageLibrary::tryGetImage(uint32_t id)
{
    if(gImagePath[id].mImage)
    {
        return gImagePath[id].mImage;
    }

    requestLoad(id);
    
    return NULL;
}

Image* ImageLibrary::getImage(uint32_t id)
{
    if(gImagePath[id].mImage == NULL)
    {
        dprintf("getImage for %u loading inline", id);
        RawImage rawImage;
        load(id, &rawImage);
        createImage(id, &rawImage);
        delete[] rawImage.mPixels;
    }

    gImagePath[id].mInUse = true;
    
    return gImagePath[id].mImage;
}

void ImageLibrary::purgeImage(uint32_t id)
{
    gImagePath[id].mInUse = false;

    if(gImagePath[id].mImage)
    {
        // Free GL Texture.
        glDeleteTextures(1, &gImagePath[id].mImage->mTexture);
        delete gImagePath[id].mImage;
        gImagePath[id].mImage = NULL;
    }
}

void ImageLibrary::purgeImage(Image* image)
{
    purgeImage(image->mTableIndex);
}

int ImageLibrary::requestLoad(uint32_t id)
{
    if(gImagePath[id].mInUse == true)
    {
        return 0;
    }

    return queueRequestLoad(id);
}

int ImageLibrary::queueRequestLoad(uint32_t id)
{
    pthread_mutex_lock(&mMutex);

    if(mFree == NULL)
    {
        pthread_mutex_unlock(&mMutex);
        return -1;
    }
    
    // Mark that this image is in use so that another request isn't issued.
    gImagePath[id].mInUse = true;

    ImageLoadRequest* curr = mFree;
    mFree = mFree->mNext;

    curr->mId = id;

    curr->mNext = mToService;
    mToService = curr;

    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mMutex);
    
    return 0;
}


void ImageLibrary::load(uint32_t id, RawImage* rawImage)
{
    if(gImagePath[id].mImage != NULL)
    {
        assert(true);
        return;
    }

    char filename[256];
    snprintf(filename, 256, "data/%s", gImagePath[id].mPath);
    dprintf("ImageLibrary is loading %d (%s)", id, filename);

    openRawImage(filename, rawImage);
}


void ImageLibrary::createImage(uint32_t id, RawImage* raw)
{
    gImagePath[id].mImage = new Image;
    gImagePath[id].mImage->mTexture = 0;
    gImagePath[id].mImage->mSize = raw->mSize;
    gImagePath[id].mImage->mTableIndex = id;

    int mode;
    if(raw->mBytesPerPixel == 3)
        mode = GL_RGB;
    else if(raw->mBytesPerPixel == 4)
        mode = GL_RGBA;
    
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &gImagePath[id].mImage->mTexture);
	glBindTexture(GL_TEXTURE_2D, gImagePath[id].mImage->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, raw->mPixelSize.x, raw->mPixelSize.y, 0, mode, GL_UNSIGNED_BYTE, raw->mPixels);
    
    // Create texture coords.
    if(raw->mPixelSize.x == raw->mSize.x && raw->mPixelSize.y == raw->mSize.y)
    {
        gImagePath[id].mImage->mTexCoords[0] = Vector2(0, 0);
        gImagePath[id].mImage->mTexCoords[1] = Vector2(1, 0);
        gImagePath[id].mImage->mTexCoords[2] = Vector2(0, 1);
        gImagePath[id].mImage->mTexCoords[3] = Vector2(1, 1);
    }
    else
    {
        Vector2 size = raw->mSize;
        Vector2 pixel = raw->mPixelSize;
        
        float right = size.x / pixel.x;
        float top = 1 - size.y / pixel.y;
        gImagePath[id].mImage->mTexCoords[0] = Vector2(0, top);
        gImagePath[id].mImage->mTexCoords[1] = Vector2(right, top);
        gImagePath[id].mImage->mTexCoords[2] = Vector2(0, 1);
        gImagePath[id].mImage->mTexCoords[3] = Vector2(right, 1);
    }
}


void ImageLibrary::update()
{
    // Preload as many as we can.
    while(mPreloadIndex<mImageCount)
    {
        // If the current image should be preloaded
        if(gImagePath[mPreloadIndex].mInUse && (gImagePath[mPreloadIndex].mImage == NULL))
        {
            // try to preload        
            if(queueRequestLoad(mPreloadIndex) <0)
                break;
        }
        // Go to next image.
        ++mPreloadIndex;
    }
    
    // Make observer callback if we haven't already.
    if(mPreloadIndex == mImageCount && !mPreloadComplete)
    {
        mPreloadComplete = true;
        if(mObserver) mObserver->onPreloadComplete();
    }

    pthread_mutex_lock(&mMutex);

    while(mToNotify)
    {
        ImageLoadRequest* curr = mToNotify;
        mToNotify = curr->mNext;

        if(gImagePath[curr->mId].mInUse == true)
        {
            createImage(curr->mId, &curr->mRawImage);
        }
        delete[] curr->mRawImage.mPixels;

        curr->mNext = mFree;
        mFree = curr;
    }

    pthread_mutex_unlock(&mMutex);
}

void* ImageLibrary::threadBootFunc(void* arg)
{
    ((ImageLibrary*)arg)->threadFunc();
    return NULL;
}

void ImageLibrary::threadFunc()
{
    pthread_mutex_lock(&mMutex);
    while(true)
    {
        if(mRunning == false)
        {
            pthread_mutex_unlock(&mMutex);
            return;
        }

        while(mToService)
        {
            ImageLoadRequest* curr = mToService;
            mToService = curr->mNext;
            pthread_mutex_unlock(&mMutex);

            load(curr->mId, &(curr->mRawImage));

            pthread_mutex_lock(&mMutex);

            curr->mNext = mToNotify;
            mToNotify = curr;
        }
        
        dprintf("ImageLibrary::threadFunc going to sleep.");
        pthread_cond_wait(&mCondition, &mMutex);
        dprintf("ImageLibrary::threadFunc woke up.");
    }
}


}
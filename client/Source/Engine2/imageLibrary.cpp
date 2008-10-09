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

    {"interface/backgroundMain.png", NULL, false},        // 6
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


};


Image* makeSubImage(RawImage* raw, Vector2 start, Vector2 size)
{
	Image* image = new Image;
    image->mTexture = 0;
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

    int mode;
    if(raw->mBytesPerPixel == 3)
        mode = GL_RGB;
    else if(raw->mBytesPerPixel == 4)
        mode = GL_RGBA;

    gImagePath[id].mImage->mTableIndex = id;
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &gImagePath[id].mImage->mTexture);
	glBindTexture(GL_TEXTURE_2D, gImagePath[id].mImage->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, gImagePath[id].mImage->mSize.x, gImagePath[id].mImage->mSize.y, 0, mode, GL_UNSIGNED_BYTE, raw->mPixels);
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
    while(true)
    {
        pthread_mutex_lock(&mMutex);
        dprintf("ImageLibrary::threadFunc going to sleep.");
        pthread_cond_wait(&mCondition, &mMutex);

        dprintf("ImageLibrary::threadFunc woke up.");

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

        pthread_mutex_unlock(&mMutex);
    }
}


}
#include "platform.h"
#include "imageLibrary.h"
#include "view.h"

namespace pammo
{

ImageTexture gImagePath[] = 
{
    {"interface/iconWeaponHighlight.png", Image(), false, true},         // 0
    {"interface/iconWeaponLightningGun.png", Image(), false, true},      // 1 
    {"interface/iconWeaponGoo.png", Image(), false, true},               // 2
    {"interface/iconWeaponGrenadeLauncher.png", Image(), false, true},   // 3
    {"interface/iconShieldEnable.png", Image(), false, true},           // 4
    {"interface/iconShieldDisable.png", Image(), false, true},           // 5

    {"../Default.png", Image(), false, false},        // 6
    {"interface/backgroundTutorial.png", Image(), false, false},          // 7
    {"interface/backgroundCredits.png", Image(), false, false},           // 8

    {"interface/shieldMapLifeBar.png", Image(), false, true},            // 9
    {"interface/healthDot.png", Image(), false, true},                   // 10

    {"interface/ringMovement.png", Image(), false, true},                // 11
    {"interface/ringTarget.png", Image(), false, true},                  // 12

    {"interface/deathCards/00.png", Image(), false, false},               // 13
    {"interface/deathCards/01.png", Image(), false, false},               // 14
    {"interface/deathCards/02.png", Image(), false, false},               // 15
    {"interface/deathCards/03.png", Image(), false, false},               // 16
    {"interface/deathCards/04.png", Image(), false, false},               // 17
    {"interface/deathCards/05.png", Image(), false, false},               // 18
    {"interface/deathCards/06.png", Image(), false, false},               // 19
    {"interface/deathCards/07.png", Image(), false, false},               // 20
    {"interface/deathCards/08.png", Image(), false, false},               // 21
    {"interface/deathCards/09.png", Image(), false, false},               // 22
    {"interface/deathCards/10.png", Image(), false, false},               // 23

    {"vehicles/tank/00.png", Image(), false, true},                      // 24
    {"vehicles/tank/01.png", Image(), false, true},                      // 25
    {"vehicles/tank/02.png", Image(), false, true},                      // 26
    {"vehicles/tank/turret/00.png", Image(), false, true},               // 27

    {"fonts/caslonAntique/0.png", Image(), false, true},                 // 28
    {"fonts/caslonAntique/1.png", Image(), false, true},                 // 29
    {"fonts/caslonAntique/2.png", Image(), false, true},                 // 30
    {"fonts/caslonAntique/3.png", Image(), false, true},                 // 31
    {"fonts/caslonAntique/4.png", Image(), false, true},                 // 32
    {"fonts/caslonAntique/5.png", Image(), false, true},                 // 33
    {"fonts/caslonAntique/6.png", Image(), false, true},                 // 34
    {"fonts/caslonAntique/7.png", Image(), false, true},                 // 35
    {"fonts/caslonAntique/8.png", Image(), false, true},                 // 36
    {"fonts/caslonAntique/9.png", Image(), false, true},                 // 37

    {"particles/flame/00.png", Image(), false, true},                    // 38

    {"particles/heatSeaker/00.png", Image(), false, true},               // 39
    {"particles/heatSeaker/tail/00.png", Image(), false, true},          // 40

    {"particles/mine.png", Image(), false, true},                        // 41

    {"particles/ball.png", Image(), false, true},                        // 42
    {"particles/shadow/00.png", Image(), false, true},                      // 43

    {"particles/machineGun/00.png", Image(), false, true},               // 44
    {"particles/machineGun/01.png", Image(), false, true},               // 45
    {"particles/machineGun/02.png", Image(), false, true},               // 46

    {"particles/goo/00.png", Image(), false, true},                      // 47
    {"particles/goo/01.png", Image(), false, true},                      // 48
    {"particles/goo/02.png", Image(), false, true},                      // 49

    {"particles/smoke/00.png", Image(), false, true},                    // 50

    {"particles/dust/00.png", Image(), false, true},                     // 51

    {"particles/rubble/00.png", Image(), false, true},                   // 52

    {"particles/lightningGlow/00.png", Image(), false, true},            // 53

    {"particles/lightning/00.png", Image(), false, true},                // 54
    {"particles/lightning/01.png", Image(), false, true},                // 55
    {"particles/lightning/02.png", Image(), false, true},                // 56

    {"particles/grenade/00.png", Image(), false, true},                  // 57

    {"particles/explosion/00.png", Image(), false, true},                // 58
    {"particles/explosion/01.png", Image(), false, true},                // 59

    {"particles/shield/00.png", Image(), false, true},                   // 60
    {"particles/shield/01.png", Image(), false, true},                   // 61
    {"particles/shield/02.png", Image(), false, true},                   // 62

    {"particles/fakeWeapon.png", Image(), false, true},                  // 63

    {"vehicles/bigBoy/00.png", Image(), false, true},                    // 64

    {"vehicles/flameTank/00.png", Image(), false, true},                 // 65
    {"vehicles/flameTank/01.png", Image(), false, true},                 // 66
    {"vehicles/flameTank/02.png", Image(), false, true},                 // 67
    {"vehicles/flameTank/03.png", Image(), false, true},                 // 68
    {"vehicles/flameTank/04.png", Image(), false, true},                 // 69
    {"vehicles/flameTank/turret/00.png", Image(), false, true},          // 70

    {"vehicles/killerTop/00.png", Image(), false, true},                 // 71

    {"vehicles/mineLayer/00.png", Image(), false, true},                 // 72

    {"vehicles/torpedo/00.png", Image(), false, true},                   // 73

    {"vehicles/trebuchet/00.png", Image(), false, true},                 // 74

    {"particles/jetFlame/00.png", Image(), false, true},                 // 75

    {"interface/buttonMask.png", Image(), false, true},                  // 76
    
    {"particles/powerupLifeUpgrade.png", Image(), false, true},              // 77
    {"particles/powerupLifeRestore.png", Image(), false, true},              // 78
    {"particles/powerupEnergyUpgrade.png", Image(), false, true},            // 79
    {"particles/powerupEnergyRestore.png", Image(), false, true},            // 80
    {"particles/powerupShield.png", Image(), false, true},                   // 81
    {"particles/powerupGooWeapon.png", Image(), false, true},                // 82
    {"particles/powerupGrenadeLauncherWeapon.png", Image(), false, true},    // 83
    
    {PAMMO_MAP_PREVIEW_PATH, Image(), false, true},    // 84
    
    #if 0
    // Alloc a bunch of empty entries, 6 x 4.
    {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false},
    {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false},
    {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false},
    {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false}, {"", Image(), false, false},
    #else
    {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true},
    {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true},
    {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true},
    {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true}, {"", Image(), false, true},
    #endif
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
    spawnThread(threadBootFunc, this);
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
    if(gImagePath[id].mBound)
    {
        return &gImagePath[id].mImage;
    }

    requestLoad(id);
    
    return NULL;
}

Image* ImageLibrary::getImage(uint32_t id)
{
    if(!gImagePath[id].mBound)
    {
        dprintf("getImage for %u loading inline", id);
        RawImage rawImage;
        load(id, &rawImage);
        createImage(id, &rawImage);
        delete[] rawImage.mPixels;
    }

    gImagePath[id].mInUse = true;
    
    return &gImagePath[id].mImage;
}

void ImageLibrary::purgeImage(uint32_t id)
{
    gImagePath[id].mInUse = false;

    if(gImagePath[id].mBound)
    {
        // Free GL Texture.
        glDeleteTextures(1, &gImagePath[id].mImage.mTexture);
        gImagePath[id].mBound = false;
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
    if(gImagePath[id].mBound)
    {
        assert(true);
        return;
    }

    char filename[256];
    if(id < MAP_TILES_BASE)
    {
        // If this is not a map tile, load the path from provided path.
        snprintf(filename, sizeof(filename), "data/%s", gImagePath[id].mPath);
    }
    else
    {
        // Otherwise, manually construct the filepath.
        uint32_t x = (id - MAP_TILES_BASE) % MAP_TILES_X;
        uint32_t y = (id - MAP_TILES_BASE) / MAP_TILES_X;
        snprintf(filename, sizeof(filename), PAMMO_MAP_TILES_FORMAT, x, y);
    }
    dprintf("ImageLibrary is loading %d (%s)", id, filename);

    openRawImage(filename, rawImage);
}


void ImageLibrary::createImage(uint32_t id, RawImage* raw)
{
    gImagePath[id].mBound = true;
    gImagePath[id].mImage.mSize = raw->mSize;
    gImagePath[id].mImage.mTableIndex = id;
    
    bool compressed;

    int mode, type;
    switch(raw->mBytesPerPixel)
    {
        case 4:
            dprintf("Binding 32bit image");
            compressed = false;
            type = GL_UNSIGNED_BYTE;
            mode = GL_RGBA;
            break;
        case 3:
            dprintf("Binding 24bit image");
            compressed = false;
            type = GL_UNSIGNED_BYTE;
            mode = GL_RGB;
            break;
        case 2:
            dprintf("Binding 16bit image");
            compressed = false;
            type = PAMMO_GL_16BIT;
            mode = GL_RGB;
            break;
        case 1:
            dprintf("Binding compressed rgb image");
            compressed = true;
            mode = PAMMO_GL_8BIT_RGA;
            break;
        case 0:
            dprintf("Binding compressed rgba image");
            compressed = true;
            mode = PAMMO_GL_8BIT_RGBA;
            break;
        default:
            assert(0);
    }
    
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &gImagePath[id].mImage.mTexture);
	glBindTexture(GL_TEXTURE_2D, gImagePath[id].mImage.mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    //uint64_t start = getTime();
    if(compressed)
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, mode, raw->mPixelSize.x, raw->mPixelSize.y, 0, raw->mPixelSize.x*raw->mPixelSize.y/2, raw->mPixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, mode, raw->mPixelSize.x, raw->mPixelSize.y, 0, mode, type, raw->mPixels);
    //dprintf("Binding took %d", (uint32_t)(getTime() - start));
    
    // Create texture coords.
    if(raw->mPixelSize.x == raw->mSize.x && raw->mPixelSize.y == raw->mSize.y)
    {
        gImagePath[id].mImage.mTexCoords[0] = Vector2(0, 0);
        gImagePath[id].mImage.mTexCoords[1] = Vector2(1, 0);
        gImagePath[id].mImage.mTexCoords[2] = Vector2(0, 1);
        gImagePath[id].mImage.mTexCoords[3] = Vector2(1, 1);
    }
    else
    {
        Vector2 size = raw->mSize;
        Vector2 pixel = raw->mPixelSize;
        
        float right = size.x / pixel.x;
        float top = 1 - size.y / pixel.y;
        gImagePath[id].mImage.mTexCoords[0] = Vector2(0, top);
        gImagePath[id].mImage.mTexCoords[1] = Vector2(right, top);
        gImagePath[id].mImage.mTexCoords[2] = Vector2(0, 1);
        gImagePath[id].mImage.mTexCoords[3] = Vector2(right, 1);
    }
}

void ImageLibrary::update()
{
    // Preload as many as we can.
    while(mPreloadIndex<mImageCount)
    {
        // If the current image should be preloaded
        if(gImagePath[mPreloadIndex].mInUse && (gImagePath[mPreloadIndex].mBound == false))
        {
            // try to preload
            if(queueRequestLoad(mPreloadIndex) < 0)
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
    return 0;
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
        
        //dprintf("ImageLibrary::threadFunc going to sleep.");
        pthread_cond_wait(&mCondition, &mMutex);
        //dprintf("ImageLibrary::threadFunc woke up.");
    }
}


}
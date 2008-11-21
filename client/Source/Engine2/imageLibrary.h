#ifndef __IMAGE_LIBRARY_H__
#define __IMAGE_LIBRARY_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{
    
struct Image;


// Interface

#define INTERFACE_ICON_WEAPON_HIGHLIGHT             (uint32_t)0
#define INTERFACE_ICON_WEAPON_LIGHTNINGGUN          1
#define INTERFACE_ICON_WEAPON_GOO                   2
#define INTERFACE_ICON_WEAPON_GRENADELAUNCHER       3

#define INTERFACE_ICON_SHELD_ENABLE                 4
#define INTERFACE_ICON_SHELD_DISABLE                5

#define INTERFACE_BACKGROUND_MAIN0                  6
#define INTERFACE_BACKGROUND_TUTORIAL0              7
#define INTERFACE_BACKGROUND_CREDITS                8

#define INTERFACE_MINIMAP                           9
#define INTERFACE_HEALTHDOT                         10

#define INTERFACE_RING_MOVEMENT                     11
#define INTERFACE_RING_TARGET                       12


#define INTERFACE_DEATH_CARD_COUNT              10
#define INTERFACE_DEATH_CARD_00                     13
#define INTERFACE_DEATH_CARD_01                     14
#define INTERFACE_DEATH_CARD_02                     15
#define INTERFACE_DEATH_CARD_03                     16
#define INTERFACE_DEATH_CARD_04                     17
#define INTERFACE_DEATH_CARD_05                     18
#define INTERFACE_DEATH_CARD_06                     19
#define INTERFACE_DEATH_CARD_07                     20
#define INTERFACE_DEATH_CARD_08                     21
#define INTERFACE_DEATH_CARD_09                     22
#define INTERFACE_DEATH_CARD_10                     23



// Vehicles
#define VEHICLE_TANK_COUNT                      6
#define VEHICLE_TANK_00                             24
#define VEHICLE_TANK_01                             25
#define VEHICLE_TANK_02                             26
#define VEHICLE_TANK_03                             27
#define VEHICLE_TANK_04                             28
#define VEHICLE_TANK_05                             29
    
#define VEHICLE_TANK_TURRET_00                      30


// Fonts
#define FONT_CASLONANTIQUE_0                        31
#define FONT_CASLONANTIQUE_1                        32
#define FONT_CASLONANTIQUE_2                        33
#define FONT_CASLONANTIQUE_3                        34
#define FONT_CASLONANTIQUE_4                        35
#define FONT_CASLONANTIQUE_5                        36
#define FONT_CASLONANTIQUE_6                        37
#define FONT_CASLONANTIQUE_7                        38
#define FONT_CASLONANTIQUE_8                        39
#define FONT_CASLONANTIQUE_9                        40


// Particles
#define PARTICLE_FLAME_00                           41
#define PARTICLE_HEATSEAKER_00                      42
#define PARTICLE_HEATSEAKER_TAIL_00                 43
#define PARTICLE_MINE_00                            44
#define PARTICLE_MINE_01                            45
#define PARTICLE_BALL_00                            46
#define PARTICLE_SHADOW_00                          47


#define PARTICLE_MACHINEGUN_COUNT               3
#define PARTICLE_MACHINEGUN_00                      48
#define PARTICLE_MACHINEGUN_01                      49
#define PARTICLE_MACHINEGUN_02                      50


#define PARTICLE_GOO_COUNT                      3
#define PARTICLE_GOO_00                             51
#define PARTICLE_GOO_01                             52
#define PARTICLE_GOO_02                             53

#define PARTICLE_SMOKE_00                           54
#define PARTICLE_DUST_00                            55
#define PARTICLE_RUBBLE_00                          56

#define PARTICLE_LIGHTNING_GLOW_00                  57

#define PARTICLE_LIGHTNING_COUNT               3
#define PARTICLE_LIGHTNING_00                       58
#define PARTICLE_LIGHTNING_01                       59
#define PARTICLE_LIGHTNING_02                       60

#define PARTICLE_GRENADE_00                         61

#define PARTICLE_EXPLOSION_COUNT               2
#define PARTICLE_EXPLOSION_00                       62
#define PARTICLE_EXPLOSION_01                       63

#define PARTICLE_SHIELD_COUNT                  3
#define PARTICLE_SHIELD_00                          64
#define PARTICLE_SHIELD_01                          65
#define PARTICLE_SHIELD_02                          66

#define PARTICLE_FAKE_WEAPON                        67

#define VEHICLE_BIGBOY_COUNT                  5
#define VEHICLE_BIGBOY_00                           68
#define VEHICLE_BIGBOY_01                           69
#define VEHICLE_BIGBOY_02                           70
#define VEHICLE_BIGBOY_03                           71
#define VEHICLE_BIGBOY_04                           72

#define VEHICLE_FLAMETANK_COUNT               5
#define VEHICLE_FLAMETANK_00                        73
#define VEHICLE_FLAMETANK_01                        74
#define VEHICLE_FLAMETANK_02                        75
#define VEHICLE_FLAMETANK_03                        76
#define VEHICLE_FLAMETANK_04                        77

#define VEHICLE_FLAMETANK_TURRET_00                 78

#define VEHICLE_KILLERTOP_00                        79

#define VEHICLE_MINELAYER_COUNT               4
#define VEHICLE_MINELAYER_00                        80
#define VEHICLE_MINELAYER_01                        81
#define VEHICLE_MINELAYER_02                        82
#define VEHICLE_MINELAYER_03                        83

#define VEHICLE_TORPEDO_00                          84

#define VEHICLE_TREBUCHET_COUNT               9
#define VEHICLE_TREBUCHET_00                        85
#define VEHICLE_TREBUCHET_01                        86
#define VEHICLE_TREBUCHET_02                        87
#define VEHICLE_TREBUCHET_03                        88
#define VEHICLE_TREBUCHET_04                        89
#define VEHICLE_TREBUCHET_05                        90
#define VEHICLE_TREBUCHET_06                        91
#define VEHICLE_TREBUCHET_07                        92
#define VEHICLE_TREBUCHET_08                        93

#define PARTICLE_JETFLAME_00                        94

#define INTERFACE_BUTTONMASK                        95

#define POWERUP_LIFE_UPGRADE                        96

#define POWERUP_LIFE_RESTORE                        97

#define POWERUP_ENERGY_UPGRADE                      98

#define POWERUP_ENERGY_RESTORE                      99

#define POWERUP_SHIELD                              100

#define POWERUP_GOO_WEAPON                          101

#define POWERUP_GRENADE_LAUNCHER_WEAPON             102

#define INTERFACE_OVERLAY                           103
#define INTERFACE_SCOREGLOW                         104
#define INTERFACE_BAR_METER                         105

#define INTERFACE_BACKGROUND_TUTORIAL1              106
#define INTERFACE_BACKGROUND_TUTORIAL2              107
#define INTERFACE_BACKGROUND_MAIN1                  108
#define INTERFACE_BACKGROUND_MAIN2                  109
    

#define MAP_PREVIEW                                 110

//#define MAP_TILES_X              22
//#define MAP_TILES_Y              15
//#define MAP_TILES_X              11
//#define MAP_TILES_Y              8
#define MAP_TILES_X              6
#define MAP_TILES_Y              4
#define MAP_TILES_BASE           111

#define NUM_IMAGES (MAP_TILES_BASE + MAP_TILES_X*MAP_TILES_Y)

struct ImageTexture
{
    char const* mPath;
    Image*      mImage;
    bool        mBound;
    bool        mInUse;
};

extern ImageTexture gImagePath[];

class ImageLibraryObserver
{
public:
    virtual ~ImageLibraryObserver(){}
    virtual void onImagePreloadComplete() = 0;
};


// Caveats:
//   Don't use tryGetImage and getImage on the same image. Race conditions can occur.
//   Make sure preload is complete before using getImage on images that are preloaded.
//   Make all of your calls from the same thread.
class ImageLibrary : public View
{
public:
    ImageLibrary();
    virtual ~ImageLibrary();

    void setObserver(ImageLibraryObserver* o);
    virtual void update();
    virtual uint32_t getUpdatePriority() const;

    // Blocking, inline load for image.
    Image* getImage(uint32_t id);

    // Returns image if loaded and available.
    // Returns NULL if not available.
    Image* tryGetImage(uint32_t id);

    // Notes that the image can be unloaded from memory.
    void purgeImage(uint32_t id);
    void purgeImage(Image* image);

    
private:
    // Request that an image be loaded.
    // Returns zero if request will be honored or if already requested.
    // Returns -1 if request queue is full. Try again later.
    int requestLoad(uint32_t id);

    // Queues a load request. 
    int queueRequestLoad(uint32_t id);

    uint32_t mImageCount;
    uint32_t mPreloadIndex;
    bool     mPreloadComplete;


    struct ImageLoadRequest
    {
        uint32_t          mId;
        ImageLoadRequest* mNext;
        RawImage          mRawImage;
    };

    static const uint32_t kImageLoadStackSize = 20;
    ImageLoadRequest* mFree;
    ImageLoadRequest* mToService;
    ImageLoadRequest* mToNotify;

    // Load the requested image if not already loaded.
    void load(uint32_t id, RawImage* rawImage);

    // Creates the Image for the image at the specified index in the table.
    void createImage(uint32_t id, RawImage* rawImage);

    pthread_t        mThread;
    pthread_mutex_t  mMutex;
    pthread_cond_t   mCondition;

    static void* threadBootFunc(void* arg);
    void threadFunc();

    bool mRunning;

    ImageLibraryObserver* mObserver;
};
    
extern ImageLibrary* gImageLibrary;
    
}

#endif
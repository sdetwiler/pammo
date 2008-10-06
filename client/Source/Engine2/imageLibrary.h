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

#define INTERFACE_BACKGROUND_MAIN                   6
#define INTERFACE_BACKGROUND_TUTORIAL               7
#define INTERFACE_BACKGROUND_CREDITS                8

#define INTERFACE_SHEILD_MAP_LIFEBAR                9
#define INTERFACE_HEALTHDOT                         10

#define INTERFACE_RING_MOVEMENT                     11
#define INTERFACE_RING_TARGET                       12


#define INTERFACE_DEATH_CARD_COUNT              11
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
#define VEHICLE_TANK_COUNT                      3
#define VEHICLE_TANK_00                             24
#define VEHICLE_TANK_01                             25
#define VEHICLE_TANK_02                             26
#define VEHICLE_TANK_TURRET_00                      27


// Fonts
#define FONT_CASLONANTIQUE_0                        28
#define FONT_CASLONANTIQUE_1                        29
#define FONT_CASLONANTIQUE_2                        30
#define FONT_CASLONANTIQUE_3                        31
#define FONT_CASLONANTIQUE_4                        32
#define FONT_CASLONANTIQUE_5                        33
#define FONT_CASLONANTIQUE_6                        34
#define FONT_CASLONANTIQUE_7                        35
#define FONT_CASLONANTIQUE_8                        36
#define FONT_CASLONANTIQUE_9                        37


// Particles
#define PARTICLE_FLAME_00                           38
#define PARTICLE_HEATSEAKER_00                      39
#define PARTICLE_HEATSEAKER_TAIL_00                 40
#define PARTICLE_MINE                               41
#define PARTICLE_BALL_00                            42
#define PARTICLE_SHADOW_00                          43


#define PARTICLE_MACHINEGUN_COUNT               3
#define PARTICLE_MACHINEGUN_00                      44
#define PARTICLE_MACHINEGUN_01                      45
#define PARTICLE_MACHINEGUN_02                      46


#define PARTICLE_GOO_COUNT                      3
#define PARTICLE_GOO_00                             47
#define PARTICLE_GOO_01                             48
#define PARTICLE_GOO_02                             49

#define PARTICLE_SMOKE_00                           50
#define PARTICLE_DUST_00                            51
#define PARTICLE_RUBBLE_00                          52

#define PARTICLE_LIGHTNING_GLOW_00                  53

#define PARTICLE_LIGHTNING_COUNT               3
#define PARTICLE_LIGHTNING_00                       54
#define PARTICLE_LIGHTNING_01                       55
#define PARTICLE_LIGHTNING_02                       56

#define PARTICLE_GRENADE_00                         57

#define PARTICLE_EXPLOSION_COUNT               2
#define PARTICLE_EXPLOSION_00                       58
#define PARTICLE_EXPLOSION_01                       59

#define PARTICLE_SHIELD_COUNT                  3
#define PARTICLE_SHIELD_00                          60
#define PARTICLE_SHIELD_01                          61
#define PARTICLE_SHIELD_02                          62

#define PARTICLE_FAKE_WEAPON                        63

#define VEHICLE_BIGBOY_00                           64

#define VEHICLE_FLAMETANK_COUNT               5
#define VEHICLE_FLAMETANK_00                        65
#define VEHICLE_FLAMETANK_01                        66
#define VEHICLE_FLAMETANK_02                        67
#define VEHICLE_FLAMETANK_03                        68
#define VEHICLE_FLAMETANK_04                        69

#define VEHICLE_FLAMETANK_TURRET_00                 70

#define VEHICLE_KILLERTOP_00                        71

#define VEHICLE_MINELAYER_00                        72

#define VEHICLE_TORPEDO_00                          73

#define VEHICLE_TREBUCHET_00                        74

#define PARTICLE_JETFLAME_00                        75

#define INTERFACE_BUTTONMASK                        76

struct ImageTexture
{
    char const* mPath;
    Image*      mImage;
    int         mRefCount;
};

//extern char* gImagePath[];
extern ImageTexture gImagePath[];

class ImageLibraryObserver
{
public:
    virtual ~ImageLibraryObserver(){}
    virtual void onPercentLoaded(float pct) = 0;
};


class ImageLibrary : public View
{
public:
    ImageLibrary();
    ~ImageLibrary();

    void setObserver(ImageLibraryObserver* o);
    
    Image* reference(uint32_t id);
//    Image* reference(char const* path);
    void reference(Image* image);
    void unreference(Image* image);

    virtual void update();
    virtual uint32_t getUpdatePriority() const;
    
private:

    void load(uint32_t id);

    struct ImageRef
    {
        Image* mImage;
        uint32_t mRefCount;
    };
    
    typedef map< string, ImageRef*> StringImageRefMap;
    StringImageRefMap mImages;

    uint32_t mCurrImageLoad;
    uint32_t mNumImages;

    ImageLibraryObserver* mObserver;
};
    
extern ImageLibrary* gImageLibrary;
    
}

#endif
#ifndef __MAINSCREEN_VIEW_H__
#define __MAINSCREEN_VIEW_H__

#include "pammo.h"
#include "view.h"
#include "imageLibrary.h"
#include "audioLibrary.h"

namespace pammo
{

struct Image;

class MainScreenView : public View, 
                       public ImageLibraryObserver,
                       public AudioLibraryObserver
{
public:
    MainScreenView();
    virtual ~MainScreenView();
    
    virtual void update();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getUpdatePriority() const;

    virtual void onImagePreloadComplete();
    virtual void onAudioPreloadComplete();

private:
    //void selectVehicle(uint32_t type);
    
    Image* mBackground0;
    Image* mBackground1;
    Image* mBackground2;
    
    Image* mButtonMask;

    //AudioInstance* mAudio;

    float mCloudAlpha;
    float mPreloadPercent;
    bool mImagePreloadComplete;
    bool mAudioPreloadComplete;

    //struct VehicleDesc
    //{
    //    uint32_t mType;
    //    Vector2 mCenter;
    //    Vector2 mSize;
    //    Image* mImage;
    //};
    //typedef vector< VehicleDesc* > VehicleDescVector;
    //VehicleDescVector mVehicles;
    //std::string mMapName; // Selected map from map view.
};

//extern MainScreenView* gMainScreenView;
    
}

#endif
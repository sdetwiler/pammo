#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "pammo.h"
#include "view.h"
#include "targetRingWidget.h"

namespace pammo
{

class Body;
class Camera;
class ImageEntity;
class VehicleController;

class Player : 
    public View,
    public TargetRingObserver
{
    public:
        Player();
        virtual ~Player();

        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;

        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();
        virtual void draw();
        
        virtual void onTargetRingUpdated(TargetRingWidget *widget, Vector2 value);
        
        void setCenter(Vector2 center);
        Vector2 const& getCenter() const;

    private:
        TargetRingWidget* mMovementRing;
        TargetRingWidget* mTargetRing;
        
        Body* mBody;
        VehicleController* mController;
        ImageEntity* mEntity;
        
        bool mFiring;
        Vector2 mFireDirection;
};

} // namespace pammo

#endif // __PLAYER_H__
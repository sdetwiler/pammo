#ifndef __SHIELD_TOGGLE_H__
#define __SHIELD_TOGGLE_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

class ShieldToggle;
class ImageEntity;

class ShieldToggleObserver
{
    public:
        virtual void onShieldToggleUpdated(ShieldToggle* widget, bool toggle) = 0;
};

class ShieldToggle : public View
{
    public:
        ShieldToggle();
        virtual ~ShieldToggle();
    
        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
    
        void reset();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();
        virtual void draw();
        
        void setObserver(ShieldToggleObserver* observer);
        void setToggle(bool toggle);
        bool getToggle();
    
    private:
        ShieldToggleObserver* mObserver;
        
        bool mToggle;
        ImageEntity* mEntity;
        Image* mEnabledImage;
        Image* mDisabledImage;
};

}

#endif
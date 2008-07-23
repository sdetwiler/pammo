#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "pammo.h"
#include "imageEntity.h"
#include "physics.h"
#include "vehicleController.h"

namespace pammo
{

class Enemy
{
    public: 
        Enemy(Vector2 const& position);
        virtual ~Enemy();

        virtual void update();
        virtual void draw();
        void setCenter(Vector2 center);

    protected:
        ImageEntity* mEntity;
        Body* mBody;
        VehicleController* mController;

    private:
};

} // namespace pammo


#endif // __ENEMY_H__
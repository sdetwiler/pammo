#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponDamageTouch.h"

namespace pammo
{

void enemyWeaponDamageTouchCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

    if(other == gWorld->getPlayer()->mBody)
    {
        Enemy* e = (Enemy*)self->mUserArg;

        // Do not have pointer to weapon, so must find it.
        for(uint32_t i=0; i<e->mWeaponCount; ++i)
        {
            if(e->mWeapon[i].mType == DamageTouch)
            {
                SelfDestructWeaponData* data = (SelfDestructWeaponData*)&(e->mWeapon[i].mData);
                e->mController.mAcceleration = e->mController.mAcceleration * -10.0f;

                gWorld->getParticleSystem()->initSmokeParticle(e->mBody->mCenter, 0, Vector2(1.0f, 0));

                gWorld->getPlayer()->damage(Vehicle, (float)data->mDamage); 
                //e->mDamageCb(e, Vehicle, 100.0f);
                return;
            }
        }
    }
}

void enemyWeaponDamageTouchCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    return;
}

} // namespace pammo
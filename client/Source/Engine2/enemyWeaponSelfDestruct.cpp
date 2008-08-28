#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponSelfDestruct.h"

namespace pammo
{

void enemyWeaponSelfDestructCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

    if(other == gWorld->getPlayer()->mBody)
    {
        Enemy* e = (Enemy*)self->mUserArg;

        // Do not have pointer to weapon, so must find it.
        for(uint32_t i=0; i<e->mWeaponCount; ++i)
        {
            if(e->mWeapon[i].mType == SelfDestruct)
            {
                SelfDestructWeaponData* data = (SelfDestructWeaponData*)&(e->mWeapon[i].mData);
                gWorld->getPlayer()->damage(Vehicle, (float)data->mDamage); 
                e->mDamageCb(e, Vehicle, 100.0f);
                return;
            }
        }
    }
}

void enemyWeaponSelfDestructCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    return;
}

} // namespace pammo
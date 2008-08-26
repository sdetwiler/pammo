#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponTrebuchet.h"

namespace pammo
{

void enemyWeaponSelfDestructCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    SelfDestructWeaponData* data = (SelfDestructWeaponData*)(w->mData);

    float distance = fabs(magnitude(e->mBody->mCenter - gWorld->getPlayer()->getCenter()));
//    dprintf("%.2f", distance);
    if(distance < 35.0f)
    {
        e->mDamageCb(e, Vehicle, 100.0f);
        gWorld->getPlayer()->damage(Vehicle, data->mDamage); 
        return;
    }

}

} // namespace pammo
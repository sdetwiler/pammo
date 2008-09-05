#ifndef __ENEMYWEAPONMINELAYER_H__
#define __ENEMYWEAPONMINELAYER_H__

namespace pammo
{

void enemyWeaponMineLayerCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response);
void enemyWeaponMineLayerCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager);

} // namespace pammo


#endif // __ENEMYWEAPONMINE_H__
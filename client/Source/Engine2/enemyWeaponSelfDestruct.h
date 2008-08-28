#ifndef __ENEMYWEAPONSELFDESTRUCT_H__
#define __ENEMYWEAPONSELFDESTRUCT_H__
namespace pammo
{
void enemyWeaponSelfDestructCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response);
void enemyWeaponSelfDestructCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager);
}
#endif // __ENEMYWEAPONDESTRUCT_H__
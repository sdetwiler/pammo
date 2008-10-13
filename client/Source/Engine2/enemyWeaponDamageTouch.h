#ifndef __ENEMYWEAPONDAMAGETOUCH_H__
#define __ENEMYWEAPONDAMAGETOUCH_H__
namespace pammo
{
void enemyWeaponDamageTouchCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response);
void enemyWeaponDamageTouchCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager);
}
#endif // __ENEMYWEAPONDAMAGETOUCH_H__
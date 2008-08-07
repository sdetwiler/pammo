#ifndef __FLAMETANKENEMY_H__
#define __FLAMETANKENEMY_H__

#include "pammo.h"
#include "enemyManager.h"

namespace pammo
{
void flameTankEnemyInit(Enemy* e, EnemyManager* manager);
void flameTankEnemyUpdate(Enemy* e, EnemyManager* manager);
void flameTankEnemyDraw(Enemy* e, EnemyManager* manager);
void flameTankEnemyDamage(Enemy* e, ParticleType type, float amount);

} // namespace pammo


#endif // __FLAMETANKENEMY_H__
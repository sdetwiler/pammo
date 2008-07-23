#ifndef __SIDESHOOTERENEMY_H__
#define __SIDESHOOTERENEMY_H__

#include "pammo.h"
#include "enemyManager.h"

namespace pammo
{

void sideShooterEnemyInit(Enemy* e, EnemyManager* manager);
void sideShooterEnemyUpdate(Enemy* e, EnemyManager* manager);
void sideShooterEnemyDraw(Enemy* e, EnemyManager* manager);

} // namespace pammo




#endif  // __SIDESHOOTERENEMY_H__

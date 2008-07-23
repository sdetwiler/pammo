#ifndef __TREBUCHETENEMY_H__
#define __TREBUCHETENEMY_H__

#include "pammo.h"
#include "enemyManager.h"

namespace pammo
{

void trebuchetEnemyInit(Enemy* e, EnemyManager* manager);
void trebuchetEnemyUpdate(Enemy* e, EnemyManager* manager);
void trebuchetEnemyDraw(Enemy* e, EnemyManager* manager);

} // namespace pammo


#endif // __TREBUCHETENEMY_H__
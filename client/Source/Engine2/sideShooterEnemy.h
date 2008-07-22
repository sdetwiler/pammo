#ifndef __SIDESHOOTERENEMY_H__
#define __SIDESHOOTERENEMY_H__

#include "pammo.h"
#include "enemy.h"

namespace pammo
{

class SideShooterEnemy : 
    public Enemy
{
public:
    SideShooterEnemy(Vector2 const& position);
    virtual ~SideShooterEnemy();

protected:

private:
        
};


} // namespace pammo




#endif  // __SIDESHOOTERENEMY_H__

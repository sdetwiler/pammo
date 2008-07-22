#ifndef __TREBUCHETENEMY_H__
#define __TREBUCHETENEMY_H__

#include "pammo.h"
#include "enemy.h"

namespace pammo
{

class TrebuchetEnemy : 
    public Enemy
{
public:
    TrebuchetEnemy(Vector2 const& position);
    virtual ~TrebuchetEnemy();

    virtual void update();

protected:

    virtual void fire(float distance);

private:
        
};


} // namespace pammo


#endif // __TREBUCHETENEMY_H__
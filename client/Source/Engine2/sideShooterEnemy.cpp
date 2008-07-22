#include "sideShooterEnemy.h"
#include "imageLibrary.h"

namespace pammo
{

SideShooterEnemy::SideShooterEnemy(Vector2 const& position) 
    : Enemy(position)
{
    mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/sideShooter/00.png"));
}

SideShooterEnemy::~SideShooterEnemy()
{
    delete mEntity;
}

} // namespace pammo
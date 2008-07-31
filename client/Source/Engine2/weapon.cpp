#include "weapon.h"

#include "imageEntity.h"

namespace pammo
{

Weapon::Weapon()
    : mIcon()
{
}

// Define the pure virtual destructor.
Weapon::~Weapon()
{}

ImageEntity* Weapon::getIcon()
{
    return &mIcon;
}

}
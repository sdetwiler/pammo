#include "pammo.h"
#include "imageLibrary.h"
#include "world.h"
#include "flameTankVehicle.h"

#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <string>

namespace pammo
{

    FlameTankVehicle::FlameTankVehicle(World* world)
    : Vehicle(world)
{

}

FlameTankVehicle::~FlameTankVehicle()
{

}

int FlameTankVehicle::init()
{
    DIR* dir;
    dir = opendir("data/vehicles/flameTank/");
    if(!dir)
    {
        dprintf("Failed to open data/vehicles/flameTank/");
        return -1;
    }

    typedef std::set< std::string > StringSet;
    StringSet filenames;
    struct dirent* item;

    while((item = readdir(dir)) != NULL)
    {
        int len = strlen(item->d_name);
        if(!strcmp(&(item->d_name[len-3]), "png"))
        {
            filenames.insert(item->d_name);
        }
    }

    closedir(dir);

    mImages = new Image*[filenames.size()];
    int i=0;
    for(StringSet::iterator j = filenames.begin(); j!=filenames.end(); ++i, ++j)
    {
      
        char filename[256];
        snprintf(filename, 255, "data/vehicles/flameTank/%s", (*j).c_str());
        mImages[i] = gImageLibrary->reference(filename);
        if(!mImages[i])
        {
            return -1;
        }
    }
    mCenter = Vector2(350, 350);

    mCurrImageIndex = 0;

    return Vehicle::init();
}


void FlameTankVehicle::draw()
{
    Vehicle::draw();
}

void FlameTankVehicle::update(int delta)
{
    Vehicle::update(delta);

    mCurrImageIndex = (mCurrImageIndex+1)%5;
    mImage = mImages[mCurrImageIndex];
    setSize(mImage->mSize);

    getTransform();

    // Add new flame particle.
    Vector2 particleCenter = mCenter;
    particleCenter.x += (-0.1f) + (rand()%20)/10.0;
    particleCenter.y += (-0.1f) + (rand()%20)/10.0;
    float rad = 55; // Distance from center of tank to end of nozzle.
    float rot = mRotation-(90.0*0.0174532925);
   
    particleCenter.y += rad*sin(rot);
    particleCenter.x += rad*cos(rot);

    Vector2 baseVelocity;
    if(mMoving)
    {
        baseVelocity.y += mSpeed*sin(rot);
        baseVelocity.x += mSpeed*cos(rot);
    }

    mWorld->getParticleSystem()->initFireParticle(particleCenter, baseVelocity, rot);
}



} // namespace pammo
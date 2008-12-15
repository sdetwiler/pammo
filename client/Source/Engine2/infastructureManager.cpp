#include "infastructureManager.h"

#include "world.h"
#include "player.h"
#include "particleSystem.h"
#include "physics.h"
#include "minimap.h"

namespace pammo
{

InfastructureManager::InfastructureManager()
{
    reset();
}

void InfastructureManager::reset()
{
    mGivenNewPowers = 0;
    
    mNextNewPowerMultiplier = 3.0;
    mNextNewPowerScore = 1500;
    
    mNextUpgradeMultiplier = 2.8;
    mNextUpgradeScore = 2500;
    
    mNextRestoreMultiplier = 2.7;
    mNextRestoreScore = 1350;
}

uint32_t InfastructureManager::getUpdatePriority() const
{
    return kInfastructureManagerPriority;
}

uint32_t InfastructureManager::getDrawPriority() const
{
    return kInfastructureManagerPriority;
}

void InfastructureManager::update()
{
    uint32_t score = gWorld->getPlayer()->mScore;
    
    if(score >= mNextNewPowerScore)
    {
        // Choose type.
        PowerupType type = kPowerupNone;
        uint32_t count = 0;
        if(!(mGivenNewPowers & kPowerupShield)) ++ count;
        if(!(mGivenNewPowers & kPowerupGooWeapon)) ++count;
        if(!(mGivenNewPowers & kPowerupGrenadeLauncherWeapon)) ++count;
        
        uint32_t choice = rand() % count;
        
        if(!(mGivenNewPowers & kPowerupShield) && choice == 0)
            type = kPowerupShield;
        else if(!(mGivenNewPowers & kPowerupGooWeapon) && (choice == 0 || choice == 1))
            type = kPowerupGooWeapon;
        else
            type = kPowerupGrenadeLauncherWeapon;
        
        if(createPowerup(type))
        {
            mGivenNewPowers |= type;
                
            // If this was the last choice, disable future powerups.
            if(count == 1)
                mNextNewPowerScore = INT_MAX;
            else
            {
                mNextNewPowerScore *= mNextNewPowerMultiplier;
                mNextNewPowerMultiplier += 0.8;
            }
        }
    }
    
    if(score >= mNextUpgradeScore)
    {
        // Choose type.
        PowerupType type;
        if((mGivenNewPowers & kPowerupShield) && (rand() & 1))
            type = kPowerupEnergyUpgrade;
        else
            type = kPowerupLifeUpgrade;
            
        if(createPowerup(type))
        {
            mNextUpgradeScore *= mNextUpgradeMultiplier;
            mNextUpgradeMultiplier += 0.25;
        }
    }
    
    if(score >= mNextRestoreScore)
    {
        // Choose type.
        PowerupType type;
        if((mGivenNewPowers & kPowerupShield) && (rand() & 1))
            type = kPowerupEnergyRestore;
        else
            type = kPowerupLifeRestore;
            
        if(createPowerup(type))
        {
            mNextRestoreScore *= mNextRestoreMultiplier;
            mNextRestoreMultiplier += 0.23;
        }
    }

}

void InfastructureManager::draw()
{
}

void InfastructureManager::setLocation(Vector2 location)
{
    dprintf("Infastructure location %f, %f", location.x, location.y);
    mLocation = location;
}

Vector2 InfastructureManager::getLocation()
{
    return mLocation;
}

void powerupParticleCallback(Particle* p, ParticleSystem* system)
{
    // Bind to body location.
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();
    
    gWorld->getMinimap()->mark(p->mImage.mCenter, kMinimapPowerupMarker);
    
    // Fade up and grow.
    if(p->mAlpha < 1)
    {
        p->mBody->mRadius += 1;
        gWorld->getPhysics()->resortBody(p->mBody);
        p->mAlpha += 1/15.;
        
        if(p->mAlpha > 1) p->mAlpha = 1;
    }
}

void powerupFadeParticleCallback(Particle* p, ParticleSystem* system)
{
    // Bind to body location.
    p->mImage.mSize += Vector2(1, 1);
    p->mImage.makeDirty();
    
    // Fade up.
    p->mAlpha -= 0.08f;
    if(p->mAlpha <= 0)
       system->removeParticle(p);
}

void powerupCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    if(other->mProperties & kPlayerCollisionProperties)
    {
        Particle* p = (Particle*)self->mUserArg;
        // Give powerup to player.
        PowerupType* type = (PowerupType*)p->mData;
        
        gWorld->getPlayer()->givePowerup(*type);
        
        p->mCallback = powerupFadeParticleCallback;
        self->mProperties = 0;
        self->mCollideProperties = kPlayerBarrierCollisionProperties;
    }
    else
    {
        response->mBounceMe = true;
        response->mBounceThem = true;
    }
}

bool InfastructureManager::createPowerup(PowerupType type)
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2, false);
    if(!p) return false;
    
    // Configure particle.
    p->mCallback = powerupParticleCallback;
    p->mAlpha = 0;
    
    uint32_t imageId;
    switch(type)
    {
        case kPowerupShield:
            imageId = POWERUP_SHIELD;
            break;
        case kPowerupGooWeapon:
            imageId = POWERUP_GOO_WEAPON;
            break;
        case kPowerupGrenadeLauncherWeapon:
            imageId = POWERUP_GRENADE_LAUNCHER_WEAPON;
            break;
        case kPowerupLifeUpgrade:
            imageId = POWERUP_LIFE_UPGRADE;
            break;
        case kPowerupLifeRestore:
            imageId = POWERUP_LIFE_RESTORE;
            break;
        case kPowerupEnergyUpgrade:
            imageId = POWERUP_ENERGY_UPGRADE;
            break;
        case kPowerupEnergyRestore:
            imageId = POWERUP_ENERGY_RESTORE;
            break;
        default:
            assert(0);
    }
    p->mImage.setImage(gImageLibrary->getImage(imageId));
    p->mImage.mCenter = mLocation;
    p->mImage.makeDirty();
    
    // Set type.
    PowerupType* ptrType = (PowerupType*)p->mData;
    *ptrType = type;
        
    // Base properties about particle bodies.
    p->mBody->mProperties = kPowerupCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kPowerupCollisionProperties | kEnemyCollisionProperties | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = powerupCollisionCallback;
    p->mBody->mUserArg = p;
    p->mBody->mShapeCallback = 0;
    p->mBody->mDamping = 0.01;
    p->mBody->mRadius = 1;
    p->mBody->mMass = 10;
    
    float x = (float)rand()/RAND_MAX - 0.5;
    float y = (float)rand()/RAND_MAX - 0.5;
    p->mBody->mCenter = mLocation + Vector2(x*5, y*5);
    
    x = (float)rand()/RAND_MAX - 0.5;
    y = (float)rand()/RAND_MAX - 0.5;
    p->mBody->mVelocity = Vector2(x*60, y*60);
    
    return true;
}

}
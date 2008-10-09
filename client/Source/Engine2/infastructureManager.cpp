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
    mNextNewPowerScore = 0;
    
    mNextUpgradeScore = 300;
    mNextRestoreScore = 125;
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
        
        if(choice == 0 && !(mGivenNewPowers & kPowerupShield))
            type = kPowerupShield;
        else if(choice == 0 || choice == 1 && !(mGivenNewPowers & kPowerupGooWeapon))
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
                mNextNewPowerScore += 0;
                //mNextNewPowerScore += 50;
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
            mNextUpgradeScore += 1000;
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
            mNextRestoreScore += 1000;
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
    
    // Fade up.
    p->mAlpha += 0.08f;
    if(p->mAlpha > 1) p->mAlpha = 1;
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
        
        dprintf("Giving player powerup: %d", *type);
        
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
    p->mBody->mRadius = 20;
    p->mBody->mMass = 10;
    p->mBody->mCenter = mLocation;
    float x = (float)rand()/RAND_MAX - 0.5;
    float y = (float)rand()/RAND_MAX - 0.5;
    dprintf("%f, %f", x, y);
    p->mBody->mVelocity = Vector2(x*60, y*60);
    
    return true;
}

}
#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyParticleJetFlame.h"

namespace pammo
{

void enemyParticleJetFlameParticleCb(Particle* p, ParticleSystem* system)
{
    float r = ((rand()%100)/200.0f);
    p->mImage.mRotation += r;
    p->mImage.mSize *= 1.03f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.15f;
}

void enemyParticleJetFlameCb(Enemy* e, EnemyParticle* ep, EnemyManager* manager)
{
    JetFlameParticleData* data = (JetFlameParticleData*)ep->mData;

    Particle* p = NULL;

    // Get a particle.
    p = gWorld->getParticleSystem()->addParticle(2, true);
    if(!p) return;

    // Calculate center. Vehicle center, plus particle offset rotated for vehicle, plus turret rotated for direction.
    Vector2 center = e->mBody->mCenter
                     + data->mPosition
                     * Transform2::createRotation(e->mController.mRotation);

    // Setup image properties.
    p->mImage.mCenter = center;
    p->mImage.mRotation = e->mController.mRotation+(M_PI/2.0f);
    p->mImage.setImage(gImageLibrary->getImage(atol(ep->mImagePath)));
    p->mImage.makeDirty();

    // Set jet flame specific particle properties.
    p->mCallback = enemyParticleJetFlameParticleCb;
    p->mAlpha = 1.0f;
}

} // namespace pammo
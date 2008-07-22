#include "physics.h"
#include "world.h"
#include "camera.h"
#include "body.h"

namespace pammo
{

Physics::Physics() : View()
{
    return;
    for(uint32_t i=0; i < 20; ++i)
    {
        Body* b = addBody();
        b->mRadius = 15;
        b->mMass = 10;
        b->mCenter = Vector2(rand()%200, rand()%200);
        b->mDamping = 0.05;
        b->mProperties = 1;
        b->mCollideProperties = 1;
    }
}

Physics::~Physics()
{
}
    
uint32_t Physics::getUpdatePriority() const
{
    return kMapPriority+1;
}

uint32_t Physics::getDrawPriority() const
{
    return kMapPriority+1;
}
    
void Physics::update()
{
    // Add pending bodies.
    for(BodyVector::iterator i=mAddBodies.begin(); i != mAddBodies.end(); ++i)
    {
        mBodies.push_back(*i);
    }
    mAddBodies.clear();
    
    // Remove pending bodies.
    for(BodyVector::iterator i=mRemoveBodies.begin(); i != mRemoveBodies.end(); ++i)
    {
        bool found = false;
        for(BodyVector::iterator j=mBodies.begin(); j != mBodies.end(); ++j)
        {
            if(*i == *j)
            {
                mBodies.erase(j);
                found = true;
                break;
            }
        }
        assert(found);
        delete *i;
    }
    mRemoveBodies.clear();

    // Main update work.
    integrate();
    collide();
}

void Physics::draw()
{
    return;
    gWorld->getCamera()->set();
    
    // Draw debug collision information.
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(1, 0, 0, .5);
    
    for(BodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        Body* b = *i;
        
        uint32_t const num = 8;
        Vector2 points[num];
        for(uint32_t j=0; j < num; ++j)
            points[j] = b->mCenter + Vector2(b->mRadius, 0) * Transform2::createRotation(M_PI/4*j);
            
        glVertexPointer(2, GL_FLOAT, 0, (float*)points);
        glDrawArrays(GL_TRIANGLE_FAN, 0, num);
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    gWorld->getCamera()->unset();
}
        
Body* Physics::addBody()
{
    Body* body = new Body;
    mAddBodies.push_back(body);
    return body;
}

void Physics::removeBody(Body* body)
{
    mRemoveBodies.push_back(body);
}
        
void Physics::integrate()
{
    float timestep = 1./30.;
    for(BodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        Body* b = *i;

        // Integrate position;
        b->mCenter += b->mVelocity * timestep;
        b->mVelocity += b->mAcceleration * timestep;
        b->mVelocity *= 1 - b->mDamping;
        b->mAcceleration = Vector2(0, 0);
    }
}

void Physics::collide()
{
    for(BodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        Body* b1 = *i;
        
        for(BodyVector::iterator i2 = i+1; i2 != mBodies.end(); ++i2)
        {
            Body* b2 = *i2;
            
            // Only calculate if there is overlap with the collision properties.
            if(!(b1->mCollideProperties & b2->mProperties) && !(b2->mCollideProperties & b1->mProperties))
                continue;
            
            // Decide if they touch.
            Vector2 diff = b1->mCenter - b2->mCenter;
            float magsq = diff.x*diff.x + diff.y*diff.y;
            float totalRadius = b1->mRadius + b2->mRadius;
            if(magsq > totalRadius * totalRadius)
                continue;
            
            Vector2 contactNormal = normalize(diff);
            float separatingVelocity = dot((b1->mVelocity - b2->mVelocity), contactNormal);
            if(separatingVelocity > 0) continue;
            
            float totalIMass = 1/(b1->mMass) + 1/(b2->mMass);
            
            float restitution = 0.3;
            float deltaVelocity = -(separatingVelocity*restitution + separatingVelocity);
            float impulse = deltaVelocity / totalIMass;
            Vector2 impulsePerIMass = contactNormal * impulse;
            b1->mVelocity += impulsePerIMass / b1->mMass;
            b2->mVelocity -= impulsePerIMass / b2->mMass;
            
            float penetration = (b1->mRadius + b2->mRadius) - sqrt(magsq);
            Vector2 movePerIMass = contactNormal * (penetration / totalIMass);
            b1->mCenter += movePerIMass / b1->mMass;
            b2->mCenter -= movePerIMass / b2->mMass;
        }
    }
}

}
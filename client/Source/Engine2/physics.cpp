#include "physics.h"
#include "world.h"
#include "camera.h"

namespace pammo
{

Physics::Physics() : View()
{
    mBodies = 0;
    mEdges = 0;
    mAddBodies = 0;
    mRemoveBodies = 0;
    mFreed = 0;
    
    return;
    for(uint32_t i=0; i < 500; ++i)
    {
        Body* b = addBody();
        b->mRadius = 15;
        b->mMass = 10;
        b->mCenter = Vector2(rand()%500, rand()%500);
        b->mDamping = 0.05;
        b->mProperties = kPlayerCollisionProperties;
        b->mCollideProperties = kPlayerCollisionProperties;
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
    while(mAddBodies)
    {
        Body* cur = mAddBodies;
        mAddBodies = cur->mNext;
        
        cur->mNext = mBodies;
        if(cur->mNext) cur->mNext->mPrev = cur;
        mBodies = cur;
        
        cur->mEdgeNext = mEdges;
        if(cur->mEdgeNext) cur->mEdgeNext->mEdgePrev = cur;
        mEdges = cur;
        
        cur->mAdded = true;
        resortBody(cur);
    }
    
    // Remove pending bodies.
    while(mRemoveBodies)
    {
        Body* cur = mRemoveBodies;
        mRemoveBodies = cur->mRemoveNext;
        
        if(cur->mNext) cur->mNext->mPrev = cur->mPrev;
        if(cur->mPrev) cur->mPrev->mNext = cur->mNext;
        else mBodies = cur->mNext;
        
        if(cur->mEdgeNext) cur->mEdgeNext->mEdgePrev = cur->mEdgePrev;
        if(cur->mEdgePrev) cur->mEdgePrev->mEdgeNext = cur->mEdgeNext;
        else mEdges = cur->mEdgeNext;
        
        cur->mAdded = false;
        cur->mNext = mFreed;
        mFreed = cur;
    }

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
    
    Body* b = mBodies;
    while(b)
    {
        uint32_t const num = 8;
        Vector2 points[num];
        for(uint32_t j=0; j < num; ++j)
            points[j] = b->mCenter + Vector2(b->mRadius, 0) * Transform2::createRotation(M_PI/4*j);
            
        glVertexPointer(2, GL_FLOAT, 0, (float*)points);
        glDrawArrays(GL_TRIANGLE_FAN, 0, num);
        
        b = b->mNext;
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    gWorld->getCamera()->unset();
}
        
Body* Physics::addBody()
{
    Body* body;
    if(mFreed)
    {
        body = mFreed;
        mFreed = body->mNext;
    }
    else
    {
        body = new Body();
    }
    
    memset(body, 0, sizeof(Body));
    body->mNext = mAddBodies;
    mAddBodies = body;
    return body;
}

void Physics::removeBody(Body* body)
{
    body->mRemoveNext = mRemoveBodies;
    mRemoveBodies = body;
}

void Physics::resortBody(Body* body)
{
    // Don't resort if the body isn't added yet.
    if(!body->mAdded) return;
    
    // Calculate left-most edge.
    float left = body->mCenter.x - body->mRadius;
    Body* cur;
    
    // Sort down.
    cur = body->mEdgeNext;
    while(cur && left > cur->mCenter.x - cur->mRadius)
    {
        body->mEdgeNext = cur->mEdgeNext;
        cur->mEdgeNext = body;
        if(body->mEdgeNext) body->mEdgeNext->mEdgePrev = body;
        
        cur->mEdgePrev = body->mEdgePrev;
        body->mEdgePrev = cur;
        if(cur->mEdgePrev) cur->mEdgePrev->mEdgeNext = cur;
        else mEdges = cur;
        
        cur = body->mEdgeNext;    
    }
    
    // Sort up.
    cur = body->mEdgePrev;
    while(cur && left < cur->mCenter.x - cur->mRadius)
    {
        cur->mEdgeNext = body->mEdgeNext;
        body->mEdgeNext = cur;
        if(cur->mEdgeNext) cur->mEdgeNext->mEdgePrev = cur;
        
        body->mEdgePrev = cur->mEdgePrev;
        cur->mEdgePrev = body;
        if(body->mEdgePrev) body->mEdgePrev->mEdgeNext = body;
        else mEdges = body;
        
        cur = body->mEdgePrev;
    }
}
        
void Physics::integrate()
{
    float timestep = 1./30.;
    Body* b = mBodies;
    while(b)
    {
        // Integrate position;
        b->mCenter += b->mVelocity * timestep;
        b->mVelocity += b->mAcceleration * timestep;
        b->mVelocity *= 1 - b->mDamping;
        b->mAcceleration = Vector2(0, 0);
        resortBody(b);
        b = b->mNext;
    }
    
    return;
    // Verify edge list integrity.
    float edge = -1e100;
    b = mEdges;
    //dprintf("Pass");
    while(b)
    {
        assert(edge <= b->mCenter.x - b->mRadius);
        edge = b->mCenter.x - b->mRadius;
        //dprintf("edge: %f", edge);
        b = b->mEdgeNext;
    }
}

void Physics::collide()
{
    uint32_t numBodies = 0;
    uint32_t numCompares = 0;
    
    Body* b1i = mEdges;
    while(b1i)
    {
        Body* b1 = b1i;
        b1i = b1i->mEdgeNext;
        float right = b1->mCenter.x + b1->mRadius;
        ++numBodies;
        
        Body* b2i = b1->mEdgeNext;
        while(b2i)
        {
            Body* b2 = b2i;
            b2i = b2i->mEdgeNext;
            
            // If the left of b1 is larger than the right of b1, there are no more possible collisions with b1.
            if(right < b2->mCenter.x - b2->mRadius)
                break;
            ++numCompares;
            
            // Calculate the overlap of collision properties.
            bool b1HitsB2 = b1->mCollideProperties & b2->mProperties;
            bool b2HitsB1 = b2->mCollideProperties & b1->mProperties;
            
            // Only calculate if there is overlap with the collision properties.
            if(!b1HitsB2 && !b2HitsB1)
                continue;
            
            // Decide if they touch.
            Vector2 diff;
			if(b1->mCenter == b2->mCenter)
				diff = Vector2(0,.001);
			else
				diff = b1->mCenter - b2->mCenter;
            float magsq = diff.x*diff.x + diff.y*diff.y;
            float totalRadius = b1->mRadius + b2->mRadius;
            if(magsq > totalRadius * totalRadius)
                continue;
            
            // This looks like a contact. Start building up a struct.
            Contact contact;
            contact.mContactNormal = normalize(diff);
            contact.mSeparatingVelocity = dot((b1->mVelocity - b2->mVelocity), contact.mContactNormal);
            contact.mPenetrationDepth = (b1->mRadius + b2->mRadius) - sqrt(magsq);
            contact.mContactPoint = contact.mContactNormal * (b1->mRadius + b2->mRadius - contact.mPenetrationDepth)/2;
            if(contact.mSeparatingVelocity > 0) continue;
            
            // Decide how to react.
            ContactResponse b1OnB2 = { false, false };
            ContactResponse b2OnB1 = { false, false };
            bool responder = false;
            
            // Make appropriate callbacks.
            if(b1HitsB2 && b1->mBodyCallback)
            {
                responder = true;
                b1->mBodyCallback(b1, b2, &contact, &b1OnB2);
            }
            if(b2HitsB1 && b2->mBodyCallback)
            {
                responder = true;
                b2->mBodyCallback(b2, b1, &contact, &b2OnB1);
            }
            
            // Eventually, react appropriately. For now, either bounce or don't bounce.
            if(!b1OnB2.mBounceMe && !b1OnB2.mBounceThem
                && !b2OnB1.mBounceMe && !b2OnB1.mBounceThem
                && responder)
                continue;
            
            // Perform velocity response.
            float restitution = 0.3;
            float totalIMass = 1/(b1->mMass) + 1/(b2->mMass);
            float deltaVelocity = -(contact.mSeparatingVelocity*restitution + contact.mSeparatingVelocity);
            float impulse = deltaVelocity / totalIMass;
            Vector2 impulsePerIMass = contact.mContactNormal * impulse;
            b1->mVelocity += impulsePerIMass / b1->mMass;
            b2->mVelocity -= impulsePerIMass / b2->mMass;
            
            // Perform position response.
            Vector2 movePerIMass = contact.mContactNormal * (contact.mPenetrationDepth / totalIMass);
            b1->mCenter += movePerIMass / b1->mMass;
            resortBody(b1);
            
            b2->mCenter -= movePerIMass / b2->mMass;
            resortBody(b2);
        }
    }
    
    //dprintf("%f compares / body", (float)numCompares / numBodies);
}

}
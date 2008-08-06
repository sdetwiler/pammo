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
    
    mShapes = 0;
}

Physics::~Physics()
{
}
    
uint32_t Physics::getUpdatePriority() const
{
    return kPhysicsPriority;
}

uint32_t Physics::getDrawPriority() const
{
    return kPhysicsPriority;
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
    
    glColor4f(0, 1, 0, .25);
    Shape* shape = mShapes;
    while(shape)
    {
        glVertexPointer(2, GL_FLOAT, 0, (float*)shape->mPoints);
        glDrawArrays(GL_TRIANGLE_FAN, 0, shape->mNumPoints);
        
        shape = shape->mNext;
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    gWorld->getCamera()->unset();
}

Shape* Physics::addShape()
{
    Shape* shape = new Shape();
    shape->mProperties = 0;
    shape->mNumPoints = 0;
    shape->mPoints = 0;
    shape->mNext = mShapes;
    mShapes = shape;
    return shape;
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
    // Clean up the body so that it doesn't affect anything.
    body->mBodyCallback = 0;
    body->mProperties = 0;
    body->mCollideProperties = 0;
    
    // Add it to the remove list.
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
    Body* b1i = mEdges;
    while(b1i)
    {
        Body* b1 = b1i;
        b1i = b1i->mEdgeNext;
        float right = b1->mCenter.x + b1->mRadius;
        
        Body* b2i = b1->mEdgeNext;
        while(b2i)
        {
            Body* b2 = b2i;
            b2i = b2i->mEdgeNext;
            
            // If the left of b1 is larger than the right of b1, there are no more possible collisions with b1.
            if(right < b2->mCenter.x - b2->mRadius)
                break;
            
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
            ContactResponse response = { false, false };
            // If b1 had the right collision properties, it will play a part in deciding the response.
            if(b1HitsB2)
            {
                // If it doesn't define a body callback, then both bodies bounce. Otherwise the callback chooses the response.
                if(!b1->mBodyCallback)
                {
                    response.mBounceMe = true;
                    response.mBounceThem = true;
                }
                else
                    b1->mBodyCallback(b1, b2, &contact, &response);
            }
            // The same response struct is going to be used for b2. Swap me / them.
            bool tmp = response.mBounceMe;
            response.mBounceMe = response.mBounceThem;
            response.mBounceThem = tmp;
            // Same as with b1.
            if(b2HitsB1)
            {
                if(!b2->mBodyCallback)
                {
                    response.mBounceMe = true;
                    response.mBounceThem = true;
                }
                else
                    b2->mBodyCallback(b2, b1, &contact, &response);
            }
            
            // Eventually, react appropriately. For now, either bounce or don't bounce.
            if(!response.mBounceMe && !response.mBounceThem)
                continue;
            else if(response.mBounceMe && response.mBounceThem)
                twoBodyResponse(b1, b2, &contact);
            else if(!response.mBounceMe && response.mBounceThem)
                oneBodyResponse(b1, &contact);
            else if(response.mBounceMe && !response.mBounceThem)
            {
                // In this case, we are flipping the normal ordering, so reverse the contact normal.
                contact.mContactNormal = - contact.mContactNormal;
                oneBodyResponse(b2, &contact);
            }
        }
            
        // Now collide shapes.
        Shape* shape = mShapes;
        Vector2 P = b1->mCenter;
        float radiusSquared = b1->mRadius * b1->mRadius;
        while(shape)
        {
            // Iterate over each point.
            for(uint32_t i=0; i < shape->mNumPoints; ++i)
            {
                // Distance from point to edge.
                // http://www.gamedev.net/community/forums/topic.asp?topic_id=308060&whichpage=1&#1964164
                Vector2 E0 = shape->mPoints[i];
                Vector2 E1 = shape->mPoints[(i+1)%shape->mNumPoints];
                Vector2 D = P - E0;
                Vector2 E = E1 - E0;
                float e2 = dot(E, E);
                float ed = dot(E, D);
                float t  = (ed / e2);
                
                if(t < 0) t = 0;
                else if(t > 1) t = 1;

                Vector2 Q = E0 + E*t;
                Vector2 PQ = Q - P;
                float dist2 = dot(PQ, PQ);
                
                // If the distance is more than radius squared, no hit.
                if(dist2 > radiusSquared) continue;
                
                // Fill out a contact and perform one body response.
                Contact contact;
                contact.mContactPoint = Q;
                contact.mContactNormal = normalize(P - Q);
                contact.mSeparatingVelocity = dot(b1->mVelocity, contact.mContactNormal);
                contact.mPenetrationDepth = b1->mRadius - sqrt(dist2);
                oneBodyResponse(b1, &contact);
            }
            
            shape = shape->mNext;
        }
    }
}

void Physics::oneBodyResponse(Body* b1, Contact* contact)
{
    // Perform velocity response.
    float restitution = 0.3;
    float totalIMass = 1/(b1->mMass);
    float deltaVelocity = -(contact->mSeparatingVelocity*restitution + contact->mSeparatingVelocity);
    float impulse = deltaVelocity / totalIMass;
    Vector2 impulsePerIMass = contact->mContactNormal * impulse;
    b1->mVelocity += impulsePerIMass / b1->mMass;
    
    // Perform position response.
    Vector2 movePerIMass = contact->mContactNormal * (contact->mPenetrationDepth / totalIMass);
    b1->mCenter += movePerIMass / b1->mMass;
    resortBody(b1);
}

void Physics::twoBodyResponse(Body* b1, Body* b2, Contact* contact)
{
    // Perform velocity response.
    float restitution = 0.3;
    float totalIMass = 1/(b1->mMass) + 1/(b2->mMass);
    float deltaVelocity = -(contact->mSeparatingVelocity*restitution + contact->mSeparatingVelocity);
    float impulse = deltaVelocity / totalIMass;
    Vector2 impulsePerIMass = contact->mContactNormal * impulse;
    b1->mVelocity += impulsePerIMass / b1->mMass;
    b2->mVelocity -= impulsePerIMass / b2->mMass;
    
    // Perform position response.
    Vector2 movePerIMass = contact->mContactNormal * (contact->mPenetrationDepth / totalIMass);
    b1->mCenter += movePerIMass / b1->mMass;
    resortBody(b1);
    
    b2->mCenter -= movePerIMass / b2->mMass;
    resortBody(b2);
}

}
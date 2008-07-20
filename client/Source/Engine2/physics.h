#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Body;

class Physics : public View
{
    public:
        Physics();
        ~Physics();
    
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
    
        virtual void update();
        virtual void draw();
        
        void addBody(Body* body);
        
    private:
        typedef vector< Body* > BodyVector;
        BodyVector mBodies;
        
        void integrate();
        void collide();
};

}

#endif

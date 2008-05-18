#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "myView.h"

class Application
{
public:
    Application();
    virtual ~Application();
    
    void run();


protected:
    MyView* mView;

private:
};


#endif // __APPLICATION_H__
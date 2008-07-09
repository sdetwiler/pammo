#ifndef __TIMERPLATFORM_H__
#define __TIMERPLATFORM_H__

#include "types_platform.h"

struct Timer
{
    struct _timeb timeBuffer;
    LARGE_INTEGER now;
    LARGE_INTEGER freq;
    uint64_t startTime;
};

int initTimer(Timer* timer);
uint64_t getTime(Timer* timer);


#endif // __TIMER_H__
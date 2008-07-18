#ifndef __TIMERPLATFORM_H__
#define __TIMERPLATFORM_H__

#include "types_platform.h"

#include <sys/time.h>

static inline uint64_t getTime(void)
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000 + t.tv_usec;
}

#endif // __TIMER_H__
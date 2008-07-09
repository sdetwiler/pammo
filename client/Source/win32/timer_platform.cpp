#include "timer_platform.h"
#include "dprintf.h"

int initTimer(Timer* timer)
{
    _ftime(&timer->timeBuffer);
    timer->startTime = (uint64_t)timer->timeBuffer.time * 1000000 + (uint64_t)timer->timeBuffer.millitm * 1000;
    if(!QueryPerformanceFrequency(&timer->freq))
    {
        dprintf("QueryPerformanceFrequency failed");
        return -1;
    }
    if(!QueryPerformanceCounter(&timer->now))
    {
        dprintf("QueryPerformanceCounter failed");
        return -1;
    }
    
    return 0;
}

uint64_t getTime(Timer* timer)
{
    QueryPerformanceCounter(&timer->now);
    return timer->startTime + ((uint64_t)timer->now.QuadPart * 1000000) / (uint64_t)timer->freq.QuadPart;
}


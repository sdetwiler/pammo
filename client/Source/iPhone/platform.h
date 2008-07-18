#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#import <OpenGLES/ES1/gl.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/time.h>

static inline uint64_t getTime(void)
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000 + t.tv_usec;
}

#endif // __PLATFORM_H__
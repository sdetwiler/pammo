#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// Memory debugging.
#include <crtdbg.h>


#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>

// This fixes the lack of stdint.h
#include <sdl_config_win32.h>

#include <gl/gl.h>

#include <direct.h>

#define PAMMO_GL_16BIT GL_UNSIGNED_BYTE

#define glOrthof  glOrtho
#define snprintf  _snprintf
#define getcwd    _getcwd
#define chdir     _chdir
#define roundf    ceil

#include <SDL.h>

#include <dirent.h>

struct Timer
{
    struct _timeb timeBuffer;
    LARGE_INTEGER now;
    LARGE_INTEGER freq;
    uint64_t startTime;
};

extern Timer gTimer;


#endif // __PLATFORM_H__
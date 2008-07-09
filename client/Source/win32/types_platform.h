#ifndef __TYPES_PLATFORM_H__
#define __TYPES_PLATFORM_H__

#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>

// This fixes the lack of stdint.h
#include <sdl_config_win32.h>

#include <gl/gl.h>

#include <direct.h>

#define glOrthof  glOrtho
#define snprintf  _snprintf
#define getcwd    _getcwd
#define chdir     _chdir

#include <SDL.h>

#include <dirent.h>



#endif // __TYPES_PLATFORM_H__
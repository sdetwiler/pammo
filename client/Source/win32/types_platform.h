#ifndef __TYPES_PLATFORM_H__
#define __TYPES_PLATFORM_H__

// This fixes the lack of stdint.h
#include <sdl_config_win32.h>
#include <windows.h>
#include <gl/gl.h>

#define glOrthof glOrtho
#define snprintf _snprintf

#include <SDL.h>

#include <dirent.h>

#endif // __TYPES_PLATFORM_H__
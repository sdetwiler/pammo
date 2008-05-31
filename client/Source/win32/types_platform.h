#ifndef __TYPES_PLATFORM_H__
#define __TYPES_PLATFORM_H__

// This fixes the lack of stdint.h
#include <sdl_config_win32.h>
#include <windows.h>
#include <gl/gl.h>

#define glOrthof glOrtho

#include <SDL.h>

#endif // __TYPES_PLATFORM_H__
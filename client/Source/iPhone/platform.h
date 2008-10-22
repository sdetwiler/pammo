#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include <dirent.h>
#include <stdint.h>
#include <sys/time.h>

#define PAMMO_GL_16BIT GL_UNSIGNED_SHORT_5_6_5
#define PAMMO_GL_8BIT_RGA GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define PAMMO_GL_8BIT_RGBA GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG

#define PAMMO_MAP_PREVIEW_PATH "backdroppvrtc/paris/preview.pvrtc"
#define PAMMO_MAP_TILES_FORMAT "data/backdroppvrtc/paris/%d-%d.pvrtc"

#endif // __PLATFORM_H__
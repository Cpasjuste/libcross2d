//
// Created by cpasjuste on 08/06/2021.
//

#ifndef C2D_GL2_H
#define C2D_GL2_H

#if __GLAD__

#include "glad/glad.h"

#elif __GLEW__

#include <GL/glew.h>

#endif

#if defined(__SDL2__) && defined(__GLES2__)
#ifndef __GLAD__

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL_opengles2.h>

#ifdef ANDROID
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glIsVertexArray glIsVertexArrayOES
#endif

#endif
// ?!
#undef GL_RGBA8
#define GL_RGBA8 GL_RGBA
#undef GL_RGB565
#define GL_RGB565 GL_RGB
// ?!
#ifndef GL_QUADS
#define GL_QUADS 0x0006
#endif
#endif

#include "platforms/gl2/gl_renderer.h"
#include "platforms/gl2/gl_shaders.h"
#include "platforms/gl2/gl_texture.h"
#include "platforms/gl2/gl_texture_buffer.h"

#define C2DTexture GLTexture

#endif //C2D_GL2_H

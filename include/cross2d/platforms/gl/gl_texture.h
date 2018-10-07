//
// Created by cpasjuste on 01/12/16.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "cross2d/skeleton/texture.h"

namespace c2d {

    class GLTexture : public Texture {

    public:

        GLTexture(const char *path);

        GLTexture(const unsigned char *buffer, int bufferSize);

        GLTexture(const Vector2f &size = Vector2f(0, 0),
                  Format format = Format::RGBA8);

        ~GLTexture();

        int save(const char *path);

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFilter(Filter filter);

        void setShader(int shaderIndex);

        unsigned int texID = 0;

        unsigned char *pixels = nullptr;

    };
}

#endif //GL_TEXTURE_H

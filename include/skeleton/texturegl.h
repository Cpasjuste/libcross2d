//
// Created by cpasjuste on 01/12/16.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "skeleton/texture.h"
#include "skeleton/TinyGL/GL/tinygl.h"

namespace c2d {

    class GLTexture : public Texture {

    public:

        GLTexture(const char *path);

        GLTexture(const Vector2f &size = Vector2f(0, 0),
                  int format = C2D_TEXTURE_FMT_RGBA8);

        ~GLTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        GLuint texID = 0;

    private:

        unsigned char *pixels = NULL;
    };
}

#endif //PSP2GL_TEXTURE_H

//
// Created by cpasjuste on 01/12/16.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "skeleton/texture.h"

namespace c2d {

    class GLTexture : public Texture {

    public:

        GLTexture(const char *path);

        GLTexture(const unsigned char *buffer, int bufferSize);

        GLTexture(const Vector2f &size = Vector2f(0, 0),
                  int format = C2D_TEXTURE_FMT_RGBA8);

        ~GLTexture();

        int resize(const Vector2f &size, bool copyPixels = false);

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        //void bindBuffer(bool generate = false);
        //void unBindBuffer();

        unsigned int texID = 0;

        unsigned char *pixels = NULL;

    private:

        //unsigned int vbo = 0;

    };
}

#endif //GL_TEXTURE_H

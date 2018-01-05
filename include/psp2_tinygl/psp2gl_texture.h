//
// Created by cpasjuste on 01/12/16.
//

#ifndef PSP2GL_TEXTURE_H
#define PSP2GL_TEXTURE_H

#include "skeleton/renderer.h"
#include "psp2gl_renderer.h"

namespace c2d {

    class PSP2GLTexture : public Texture {

    public:

        PSP2GLTexture(const char *path);

        PSP2GLTexture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        ~PSP2GLTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);
    };
}

#endif //PSP2GL_TEXTURE_H

//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TINY3D_TEXTURE_H_
#define _TINY3D_TEXTURE_H_

#include <pngdec/pngdec.h>
#include <ppu-lv2.h>

#include "cross2d/skeleton/renderer.h"
#include "ps3/tiny3d/tiny3d.h"

namespace c2d {

    class TINY3DTexture : public Texture {

    public:

        TINY3DTexture(const char *path);

        TINY3DTexture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        ~TINY3DTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        //private:
        u32 *pixels;
        u32 offset;
        text_format fmt;

    private:

    };
}

#endif //_TINY3D_TEXTURE_H_

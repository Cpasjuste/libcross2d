//
// Created by cpasjuste on 01/12/16.
//

#ifndef C2D_CTR_TEXTURE_H
#define C2D_CTR_TEXTURE_H

#include "cross2d/skeleton/texture.h"
#include "citro3d.h"

namespace c2d {

    class CTRTexture : public Texture {

    public:

        CTRTexture(const std::string &path);

        CTRTexture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        ~CTRTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFilter(Filter filter);

    private:

        void tile();

        void tileSoft();

        C3D_Tex tex;
        u8 *pixels = nullptr;
        //GPU_TEXCOLOR fmt = GPU_RGB565;
        //int bpp = 4;
        //int size = 0;
    };
}

#endif //C2D_CTR_TEXTURE_H

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

        C3D_Tex tex;
    private:

        //void upload();

        void uploadSoft();

        u8 *pixels = nullptr;
    };
}

#endif //C2D_CTR_TEXTURE_H

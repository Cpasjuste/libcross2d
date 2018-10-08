//
// Created by cpasjuste on 01/12/16.
//

#ifndef _PSP2_TEXTURE_H_
#define _PSP2_TEXTURE_H_

#include "cross2d/skeleton/texture.h"
#include "vita2d.h"

namespace c2d {

    class PSP2Texture : public Texture {

    public:

        PSP2Texture(const char *path);

        PSP2Texture(const Vector2f &size = Vector2f(0, 0),
                Format format = Format::RGBA8);

        ~PSP2Texture();

        int resize(const Vector2f &size, bool copyPixels = true);

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void setFilter(Filter filter);

        void setShader(int shader);

        void applyShader();

        //private:
        vita2d_texture *tex = nullptr;
    };
}

#endif //_PSP2_TEXTURE_H_

//
// Created by cpasjuste on 01/12/16.
//

#ifndef _PSP2_TEXTURE_H_
#define _PSP2_TEXTURE_H_

#include "skeleton/texture.h"
#include "vita2d.h"

namespace c2d {

    class PSP2Texture : public Texture {

    public:

        PSP2Texture(const char *path);

        PSP2Texture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        ~PSP2Texture();

        int lock(const FloatRect &rect, void **pixels, int *pitch);

        void setFiltering(int filter);

        //private:
        vita2d_texture *tex = nullptr;
    };
}

#endif //_PSP2_TEXTURE_H_

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

        explicit PSP2Texture(const std::string &path);

        explicit PSP2Texture(const Vector2f &size = Vector2f(0, 0),
                Format format = Format::RGBA8);

        PSP2Texture(const unsigned char *buffer, int bufferSize);

        ~PSP2Texture() override;

        int resize(const Vector2f &size, bool copyPixels = true) override;

        int lock(FloatRect *rect, void **pixels, int *pitch) override;

        void setFilter(Filter filter) override;

        void setShader(int shader) override;

        void applyShader() override;

        //private:
        vita2d_texture *tex = nullptr;
    };
}

#endif //_PSP2_TEXTURE_H_

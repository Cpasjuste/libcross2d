//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL1_TEXTURE_H_
#define _SDL1_TEXTURE_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class SDL1Texture : public Texture {

    public:

        explicit SDL1Texture(const std::string &path);

        SDL1Texture(const unsigned char *buffer, int bufferSize);

        explicit SDL1Texture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        ~SDL1Texture() override;

        int lock(FloatRect *rect, void **pixels, int *pitch) override;

        void unlock() override;

        int save(const std::string &path) override;

        void setFilter(Filter filter) override;

        SDL_Surface *surface = nullptr;
        unsigned char *pixels = nullptr;
    };
}

#endif //_SDL1_TEXTURE_H_

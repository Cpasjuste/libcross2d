//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_TEXTURE_H_
#define _SDL2_TEXTURE_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class SDL2Texture : public Texture {

    public:

        SDL2Texture(const std::string &path);

        SDL2Texture(const unsigned char *buffer, int bufferSize);

        SDL2Texture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        ~SDL2Texture() override;

        int lock(FloatRect *rect, void **pixels, int *pitch) override;

        void unlock() override;

        int save(const std::string &path) override;

        void setFilter(Filter filter) override;

        SDL_Texture *tex = nullptr;
    };
}

#endif //_SDL2_TEXTURE_H_

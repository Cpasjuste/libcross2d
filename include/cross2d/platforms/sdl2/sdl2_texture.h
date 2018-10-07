//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_TEXTURE_H_
#define _SDL2_TEXTURE_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class SDL2Texture : public Texture {

    public:

        SDL2Texture(const char *path);

        SDL2Texture(const unsigned char *buffer, int bufferSize);

        SDL2Texture(const Vector2f &size = Vector2f(0, 0),
                    int format = C2D_TEXTURE_FMT_RGBA8);

        ~SDL2Texture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        int save(const char *path);

        void setFiltering(int filter);

        SDL_Texture *tex = nullptr;
    };
}

#endif //_SDL2_TEXTURE_H_

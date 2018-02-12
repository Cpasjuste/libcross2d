//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_TEXTURE_H_
#define _SDL2_TEXTURE_H_

#include "skeleton/renderer.h"

namespace c2d {

    class SDL2Texture : public Texture {

    public:

        SDL2Texture(const char *path);

        SDL2Texture(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Texture();

        int lock(const FloatRect &rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        //private:
        SDL_Texture *tex = nullptr;
    };
}

#endif //_SDL2_TEXTURE_H_

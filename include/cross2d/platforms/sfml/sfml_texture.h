//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SFML_TEXTURE_H_
#define _SFML_TEXTURE_H_

#include "cross2d/skeleton/renderer.h"
#include "sfml_renderer.h"

namespace c2d {

    class SFMLTexture : public Texture {

    public:

        SFMLTexture(const char *path);

        SFMLTexture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        ~SFMLTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        //private:
        sf::Texture texture;
        sf::Uint8 *pixels = nullptr;
    };
}

#endif //_SFML_TEXTURE_H_

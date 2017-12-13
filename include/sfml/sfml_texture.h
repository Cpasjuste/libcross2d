//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SFML_TEXTURE_H_
#define _SFML_TEXTURE_H_

#include "../skeleton/renderer.h"
#include "sfml_renderer.h"

class SFMLTexture : Texture {

public:
    SFMLTexture(Renderer *renderer, const char *path);

    SFMLTexture(Renderer *renderer, int w, int h);

    ~SFMLTexture();

    virtual void Draw(int x, int y, int w, int h, float rotation);

    virtual int Lock(const Rect &rect, void **pixels, int *pitch);

    virtual void Unlock();

    void SetFiltering(int filter);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Uint8 *pixels = nullptr;
};

#endif //_SFML_TEXTURE_H_

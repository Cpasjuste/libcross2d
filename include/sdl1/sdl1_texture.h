//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL1_TEXTURE_H_
#define _SDL1_TEXTURE_H_

#include "../skeleton/renderer.h"

class SDL1Texture : Texture {

public:
    SDL1Texture(Renderer *renderer, const char *path);

    SDL1Texture(Renderer *renderer, int w, int h);

    ~SDL1Texture();

    void Draw(int x, int y, int w, int h, float rotation);

    int Lock(const Rect &rect, void **pixels, int *pitch);

    void Unlock();

    void SetFiltering(int filter);

private:
    SDL_Surface *surface;
};

#endif //_SDL1_TEXTURE_H_

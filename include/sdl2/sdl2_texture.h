//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_TEXTURE_H_
#define _SDL2_TEXTURE_H_

#include "../skeleton/renderer.h"

class SDL2Texture : Texture {

public:
    SDL2Texture(Renderer *renderer, const char *path);

    SDL2Texture(Renderer *renderer, int w, int h);

    ~SDL2Texture();

    void Draw(int x, int y, int w, int h, float rotation);

    int Lock(const Rect &rect, void **pixels, int *pitch);

    void Unlock();

    void SetFiltering(int filter);

private:
    SDL_Texture *tex = nullptr;
};

#endif //_SDL2_TEXTURE_H_

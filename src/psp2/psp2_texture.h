//
// Created by cpasjuste on 01/12/16.
//

#ifndef _PSP2_TEXTURE_H_
#define _PSP2_TEXTURE_H_

#include <skeleton/texture.h>
#include "vita2d.h"

class PSP2Texture : Texture {

public:
    PSP2Texture(Renderer *renderer, const char *path);

    PSP2Texture(Renderer *renderer, int w, int h);

    ~PSP2Texture();

    void Draw(int x, int y, int w, int h, float rotation);

    int Lock(const Rect &rect, void **pixels, int *pitch);

    void SetFiltering(int filter);

    vita2d_texture *tex = nullptr;
};

#endif //_PSP2_TEXTURE_H_

//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TINY3D_TEXTURE_H_
#define _TINY3D_TEXTURE_H_

#include <skeleton/renderer.h>
#include <pngdec/pngdec.h>
#include <ppu-lv2.h>
#include "tiny3d.h"

class TINY3DTexture : Texture {

public:
    TINY3DTexture(Renderer *renderer, const char *path);

    TINY3DTexture(Renderer *renderer, int w, int h);

    ~TINY3DTexture();

    void Draw(int x, int y, int w, int h, float rotation);

    int Lock(const Rect &rect, void **pixels, int *pitch);

    void Unlock();

    void SetFiltering(int filter);

private:
    u32 *pixels;
    u32 offset;
    int pitch;
    int bpp = 4;
    int size = 0;
    text_format fmt;

private:

};

#endif //_TINY3D_TEXTURE_H_

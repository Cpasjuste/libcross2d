//
// Created by cpasjuste on 01/12/16.
//

#ifndef _NX_TEXTURE_H_
#define _NX_TEXTURE_H_

#include <switch.h>
#include <skeleton/texture.h>

class NXTexture : Texture {

public:
    NXTexture(const char *path);

    NXTexture(int w, int h);

    ~NXTexture();

    // TODO
    void *tex = NULL;
    u8 *pixels = NULL;
};

#endif //_NX_TEXTURE_H_

//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_TEXTURE_H_
#define _CTR_TEXTURE_H_

class Texture;

class CTRTexture : Texture {

public:
    CTRTexture(Renderer *renderer, const char *path);

    CTRTexture(Renderer *renderer, int w, int h, GPU_TEXCOLOR format = GPU_RGB565);

    ~CTRTexture();

    void Draw(int x, int y, int w, int h, float rotation);

    int Lock(const Rect &rect, void **pixels, int *pitch);

    void Unlock();

    void SetFiltering(int filter);

private:
    void Tile();

    void TileSoft();
    C3D_Tex tex;
    u8 *pixels = NULL;
    GPU_TEXCOLOR fmt = GPU_RGB565;

    int bpp = 4;
    int size = 0;
};

#endif //_CTR_TEXTURE_H_

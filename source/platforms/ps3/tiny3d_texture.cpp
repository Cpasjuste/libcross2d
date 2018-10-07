//
// Created by cpasjuste on 01/12/16.
//

#include "cross2d/c2d.h"
#include <rsx/mm.h>

using namespace c2d;

TINY3DTexture::TINY3DTexture(const char *p) : Texture(p) {

    pngData png;
    if (pngLoadFromFile(path, &png)) {
        printf("Could not create texture: pngLoadFromFile\n");
        return;
    }

    setSize(Vector2f(png.width, png.height));
    setTextureRect(IntRect(0, 0, png.width, png.height));
    pitch = png.pitch;
    bpp = pitch / png.width;
    fmt = bpp == 2 ? TINY3D_TEX_FORMAT_R5G6B5 : TINY3D_TEX_FORMAT_A8R8G8B8;
    format = bpp == 2 ? C2D_TEXTURE_FMT_RGB565 : C2D_TEXTURE_FMT_RGBA8;

    printf("TINY3DTexture: %ix%i, bpp=%i, pitch=%i\n", png.width, png.height, bpp, pitch);

    switch (bpp) {

        case 2:
            fmt = TINY3D_TEX_FORMAT_R5G6B5;
            break;

        default:
            fmt = TINY3D_TEX_FORMAT_A8R8G8B8;
    }

    pixels = (u32 *) tiny3d_AllocTexture((u32) png.width * png.height * bpp);
    if (!pixels) {
        printf("Could not create texture: tiny3d_AllocTexture\n");
    }

    memcpy(pixels, png.bmp_out, (size_t) png.width * (size_t) png.height * bpp);
    free(png.bmp_out);

    offset = tiny3d_TextureOffset(pixels);

    available = true;
}

TINY3DTexture::TINY3DTexture(const Vector2f &size, int _fmt) : Texture(size, _fmt) {

    fmt = bpp == 2 ? TINY3D_TEX_FORMAT_R5G6B5 : TINY3D_TEX_FORMAT_A8R8G8B8;

    printf("TINY3DTexture: %ix%i, bpp=%i, pitch=%i\n", (int) size.x, (int) size.y, bpp, pitch);

    pixels = (u32 *) tiny3d_AllocTexture((u32) size.x * (u32) size.y * bpp);
    if (!pixels) {
        printf("Could not allocated TINY3DTexture\n");
    }

    offset = tiny3d_TextureOffset(pixels);

    available = true;
}

int TINY3DTexture::lock(FloatRect *rect, void **pix, int *p) {

    if (!rect) {
        *pix = pixels;
    } else {
        *pix = (void *) ((uint8_t *) pixels +
                         (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void TINY3DTexture::unlock() {
}

void TINY3DTexture::setFiltering(int filter) {
}

TINY3DTexture::~TINY3DTexture() {

    if (pixels) {
        rsxFree(pixels);
    }
}

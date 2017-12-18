//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"
#include <pngdec/pngdec.h>
#include <rsx/mm.h>

TINY3DTexture::TINY3DTexture(Renderer *r, const char *p) : Texture(r, p) {

    pngData png;
    if (pngLoadFromFile(path, &png)) {
        printf("Could not create texture: pngLoadFromFile\n");
        return;
    }

    width = png.width;
    height = png.height;
    pitch = png.pitch;
    bpp = pitch / width;
    size = width * height * bpp;

    printf("TINY3DTexture: %ix%i, bpp=%i, pitch=%i, size=%i\n", width, height, bpp, pitch, size);

    switch (bpp) {

        case 2:
            fmt = TINY3D_TEX_FORMAT_R5G6B5;
            break;

        default:
            fmt = TINY3D_TEX_FORMAT_A8R8G8B8;
    }

    pixels = (u32 *) tiny3d_AllocTexture((u32) size);
    if (!pixels) {
        printf("Could not create texture: tiny3d_AllocTexture\n");
    }

    memcpy(pixels, png.bmp_out, (size_t) size);
    free(png.bmp_out);

    offset = tiny3d_TextureOffset(pixels);

    available = true;
}

TINY3DTexture::TINY3DTexture(Renderer *r, int w, int h) : Texture(r, w, h) {

    fmt = TINY3D_TEX_FORMAT_R5G6B5;
    bpp = 2;
    pitch = width * bpp;
    size = width * height * bpp;

    printf("TINY3DTexture: %ix%i, bpp=%i, pitch=%i, size=%i\n", width, height, bpp, pitch, size);

    pixels = (u32 *) tiny3d_AllocTexture((u32) size);
    if (!pixels) {
        printf("Could not allocated TINY3DTexture\n");
    }

    offset = tiny3d_TextureOffset(pixels);

    available = true;
}

void TINY3DTexture::Draw(int x, int y, int w, int h, float rotation) {

    if (!pixels) {
        return;;
    }

    tiny3d_SetTexture(0, offset, width, height, pitch, fmt, TEXTURE_LINEAR);

    float dx = w / 2.0f;
    float dy = h / 2.0f;

    MATRIX matrix;

    // rotate and translate the sprite
    matrix = MatrixRotationZ(rotation);
    matrix = MatrixMultiply(matrix, MatrixTranslation(x + dx, y + dy, 0.0f));

    // fix ModelView Matrix
    tiny3d_SetMatrixModelView(&matrix);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(-dx, -dy, 0);
    tiny3d_VertexColor(0xffffffff);
    tiny3d_VertexTexture(0.0f, 0.0f);

    tiny3d_VertexPos(dx, -dy, 0);
    tiny3d_VertexTexture(0.99f, 0.0f);

    tiny3d_VertexPos(dx, dy, 0);
    tiny3d_VertexTexture(0.99f, 0.99f);

    tiny3d_VertexPos(-dx, dy, 0);
    tiny3d_VertexTexture(0.0f, 0.99f);

    tiny3d_End();

    tiny3d_SetMatrixModelView(NULL); // set matrix identity

}

int TINY3DTexture::Lock(const Rect &rect, void **data, int *_pitch) {

    *data = pixels;
    *_pitch = pitch;
}

void TINY3DTexture::Unlock() {
}

void TINY3DTexture::SetFiltering(int filter) {
}

TINY3DTexture::~TINY3DTexture() {

    if (pixels) {
        rsxFree(pixels);
    }
}

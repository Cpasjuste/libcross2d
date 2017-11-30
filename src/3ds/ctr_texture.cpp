//
// Created by cpasjuste on 01/12/16.
//

#include <C2D.h>
#include "ctr_png.h"

#define TILE_FLAGS(inFmt, outFmt) \
    (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(inFmt) | GX_TRANSFER_OUT_FORMAT(outFmt) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

static u16 pow2(int w) {
    if (w == 0)
        return 0;
    u16 n = 2;
    while (w > n)
        n <<= 1;
    return n;
}

// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
static inline u32 morton_interleave(u32 x, u32 y) {
    u32 i = (x & 7) | ((y & 7) << 8); // ---- -210
    i = (i ^ (i << 2)) & 0x1313;      // ---2 --10
    i = (i ^ (i << 1)) & 0x1515;      // ---2 -1-0
    i = (i | (i >> 7)) & 0x3F;
    return i;
}

static inline u32 get_morton_offset(u32 x, u32 y, u32 bytes_per_pixel) {
    u32 i = morton_interleave(x, y);
    unsigned int offset = (x & ~7) * 8;
    return (i + offset) * bytes_per_pixel;
}


CTRTexture::CTRTexture(Renderer *r, const char *path) : Texture(r, path) {

    fmt = GPU_RGBA8;
    bpp = 4;

    pixels = CTRPng::Load(&width, &height, path);
    if (!pixels) {
        printf("CTRTexture: couldn't create texture (CTRPng::Load)\n");
        return;
    }

    pixels_size = width * height * bpp;

    bool res = C3D_TexInit(&tex, pow2(width), pow2(height), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = NULL;
        return;
    }

    TileSoft();
    SetFiltering(TEXTURE_FILTER_POINT);
}

CTRTexture::CTRTexture(Renderer *r, int w, int h, GPU_TEXCOLOR format) : Texture(r, w, h) {

    //fmt = GPU_RGBA8;
    fmt = format;
    bpp = fmt == GPU_RGBA8 ? 4 : 2;
    pixels_size = width * height * 2;
    pixels = (u8 *) linearAlloc((size_t) pixels_size);
    if (!pixels) {
        return;
    }

    bool res = C3D_TexInitVRAM(&tex, pow2(width), pow2(height), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = NULL;
        return;
    }

    SetFiltering(TEXTURE_FILTER_POINT);
}

void CTRTexture::Draw(int x, int y, int w, int h, float r) {

    ((CTRRenderer *) renderer)->StartDrawing(false);

    C3D_TexBind(0, &tex);

    float _w = (((float) tex.width / (float) width) * (float) w);
    float _h = (((float) tex.height / (float) height) * (float) h);

    C3D_ImmDrawBegin(GPU_TRIANGLE_STRIP);
    C3D_ImmSendAttrib(x, y, 0.5f, 0.0f);
    C3D_ImmSendAttrib(0.0f, 0.0f, 0.0f, 0.0f);

    C3D_ImmSendAttrib(x, y + _h, 0.5f, 0.0f);
    C3D_ImmSendAttrib(0.0f, 1.0f, 0.0f, 0.0f);

    C3D_ImmSendAttrib(x + _w, y, 0.5f, 0.0f);
    C3D_ImmSendAttrib(1.0f, 0.0f, 0.0f, 0.0f);

    C3D_ImmSendAttrib(x + _w, y + _h, 0.5f, 0.0f);
    C3D_ImmSendAttrib(1.0f, 1.0f, 0.0f, 0.0f);
    C3D_ImmDrawEnd();
}

int CTRTexture::Lock(const Rect &rect, void **data, int *pitch) {

    *data = pixels;
    *pitch = width * 2;

    return 0;
}

void CTRTexture::Unlock() {

    // tile buffer for 3ds...
    if (pixels) {
        Tile();
    }
}

void CTRTexture::SetFiltering(int filter) {

    GPU_TEXTURE_FILTER_PARAM param =
            filter == TEXTURE_FILTER_POINT ? GPU_NEAREST : GPU_LINEAR;

    C3D_TexSetFilter(&tex, GPU_LINEAR, param);
}

void CTRTexture::Tile() {

    GSPGPU_FlushDataCache(pixels, (u32) pixels_size);
    GSPGPU_FlushDataCache(tex.data, tex.size);


    GX_TRANSFER_FORMAT outFmt =
            fmt == GPU_RGB565 ? GX_TRANSFER_FMT_RGB565 : GX_TRANSFER_FMT_RGBA8;

    C3D_SafeDisplayTransfer(
            (u32 *) pixels,
            (u32) GX_BUFFER_DIM(width, height),
            (u32 *) tex.data,
            (u32) GX_BUFFER_DIM(tex.width, tex.height),
            (u32) TILE_FLAGS(GX_TRANSFER_FMT_RGB565, outFmt)
    );

    gspWaitForPPF();
}

void CTRTexture::TileSoft() {

    // TODO: add support for non-RGBA8 textures
    int i, j;
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {

            u32 coarse_y = static_cast<u32>(j & ~7);
            u32 dst_offset = get_morton_offset(i, j, bpp) + coarse_y * tex.width * bpp;
            u32 v = ((u32 *) pixels)[i + (height - 1 - j) * width];
            *(u32 *) ((u32) tex.data + dst_offset) = __builtin_bswap32(v); /* RGBA8 -> ABGR8 */
        }
    }
}

CTRTexture::~CTRTexture() {

    C3D_TexDelete(&tex);

    if (pixels != NULL) {
        linearFree(pixels);
    }
}

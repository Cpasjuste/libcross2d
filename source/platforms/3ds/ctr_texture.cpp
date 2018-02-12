//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"

#ifdef __CITRO3D__
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

#endif

CTRTexture::CTRTexture(Renderer *r, const char *path) : Texture(r, path) {

#ifdef __CITRO3D__
    fmt = GPU_RGBA8;
    bpp = 4;

    pixels = CTRPng::Load(&width, &height, path);
    if (!pixels) {
        printf("CTRTexture: couldn't create texture (CTRPng::Load)\n");
        return;
    }

    size = width * height * bpp;

    bool res = C3D_TexInit(&tex, pow2(width), pow2(height), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = NULL;
        return;
    }

    TileSoft();
    SetFiltering(TEXTURE_FILTER_POINT);
    available = true;
#else

#endif
}

CTRTexture::CTRTexture(Renderer *r, int w, int h, GPU_TEXCOLOR format) : Texture(r, w, h) {

#ifdef __CITRO3D__
    fmt = format;
    bpp = fmt == GPU_RGBA8 ? 4 : 2;
    size = width * height * bpp;
    pixels = (u8 *) linearAlloc((size_t) size);
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
    available = true;
#else
    pixels = (u8 *) linearAlloc((size_t) w * h * 2);
    width = w;
    height = h;
    bpp = 2;
    available = true;
#endif
}

void CTRTexture::Draw(int x, int y, int w, int h, float r) {

#ifdef __CITRO3D__
    ((CTRRenderer *) renderer)->StartDrawing(false);

    C3D_TexBind(0, &tex);

    float _w = w;//(((float) tex.width / (float) width) * (float) w);
    float _h = h;//(((float) tex.height / (float) height) * (float) h);

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
#else

#endif
}

int CTRTexture::Lock(const Rect &rect, void **data, int *pitch) {

    *data = pixels;
    *pitch = width * bpp;

    return 0;
}

void CTRTexture::Unlock() {

#ifdef __CITRO3D__
    // tile buffer for 3ds...
    if (pixels) {
        Tile();
    }
#else
    int i, j, k;
    u8 *dst_base_addr;
    u8 *src_addr, *dst_addr;

    int win_x = (400 - width) / 2;
    int win_y = (240 - height) / 2;
    int offset = (win_y + win_x * 240) * bpp;

    dst_base_addr = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    dst_base_addr += offset;

    for (i = 0; i < height; i++) {
        src_addr = pixels + ((height - i) * width) * bpp;
        for (j = 0; j < width; j++) {
            dst_addr = dst_base_addr + (i + win_y + j * 240) * bpp;
            for (k = 0; k < bpp; k++) {
                *dst_addr++ = *src_addr++;
            }
        }
    }
#endif
}

void CTRTexture::SetFiltering(int filter) {

#ifdef __CITRO3D__
    GPU_TEXTURE_FILTER_PARAM param =
            filter == TEXTURE_FILTER_POINT ? GPU_NEAREST : GPU_LINEAR;

    C3D_TexSetFilter(&tex, GPU_LINEAR, param);
#else
#endif
}

void CTRTexture::Tile() {

#ifdef __CITRO3D__
    GSPGPU_FlushDataCache(pixels, (u32) size);
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
#endif
}

void CTRTexture::TileSoft() {

#ifdef __CITRO3D__
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
#endif
}

CTRTexture::~CTRTexture() {

#ifdef __CITRO3D__
    C3D_TexDelete(&tex);

    if (pixels != NULL) {
        linearFree(pixels);
    }
#endif
}

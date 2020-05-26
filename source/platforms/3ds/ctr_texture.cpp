//
// Created by cpasjuste on 01/12/16.
//

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

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

CTRTexture::CTRTexture(const std::string &path) : Texture(path) {

    int img_w, img_h, n = 0;
    int p2_w, p2_h;

    u8 *img = stbi_load(path.c_str(), &img_w, &img_h, &n, bpp);
    if (img == nullptr) {
        printf("CTRTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    // copy img to power of 2 pixel data
    p2_w = pow2(img_w), p2_h = pow2(img_h);
    pixels = (u8 *) linearAlloc((size_t) (p2_w * p2_h * bpp));
    if (pixels == nullptr) {
        free(img);
        return;
    }

    u8 *dst = pixels;
    int dst_pitch = p2_w * bpp;
    u8 *src = img;
    int src_pitch = img_w * bpp;
    for (int i = 0; i < img_h; i++) {
        memcpy(dst, src, (size_t) img_w * bpp);
        dst += dst_pitch;
        src += src_pitch;
    }
    free(img);

    if (!C3D_TexInit(&tex, (u16) p2_w, (u16) p2_h, GPU_RGBA8)) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = nullptr;
        return;
    }

    Texture::setSize((float) img_w, (float) img_h);
    setTexture(this, true);
    pitch = tex.width * bpp;
    available = true;
    uploadSoft();
}

CTRTexture::CTRTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    int img_w, img_h, n = 0;
    int p2_w, p2_h;

    u8 *img = stbi_load_from_memory(buffer, bufferSize, &img_w, &img_h, &n, bpp);
    if (img == nullptr) {
        printf("CTRTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }

    // copy img to power of 2 pixel data
    p2_w = pow2(img_w), p2_h = pow2(img_h);
    pixels = (u8 *) linearAlloc((size_t) (p2_w * p2_h * bpp));
    if (pixels == nullptr) {
        free(img);
        return;
    }

    u8 *dst = pixels;
    int dst_pitch = p2_w * bpp;
    u8 *src = img;
    int src_pitch = img_w * bpp;
    for (int i = 0; i < img_h; i++) {
        memcpy(dst, src, (size_t) img_w * bpp);
        dst += dst_pitch;
        src += src_pitch;
    }
    free(img);

    if (!C3D_TexInit(&tex, (u16) p2_w, (u16) p2_h, GPU_RGBA8)) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = nullptr;
        return;
    }

    Texture::setSize((float) img_w, (float) img_h);
    setTexture(this, true);
    pitch = tex.width * bpp;
    available = true;
    uploadSoft();
}

CTRTexture::CTRTexture(const Vector2f &size, Format format) : Texture(size, format) {

    int p2_w = pow2((int) size.x), p2_h = pow2((int) size.y);

    pixels = (u8 *) linearAlloc((size_t) p2_w * p2_h * bpp);
    if (pixels == nullptr) {
        return;
    }

    GPU_TEXCOLOR fmt = format == Format::RGB565 ? GPU_RGB565 : GPU_RGBA8;

    bool res = C3D_TexInit(&tex, (u16) p2_w, (u16) p2_h, fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = nullptr;
        return;
    }

    setTexture(this, true);
    pitch = tex.width * bpp;
    available = true;
}

int CTRTexture::lock(FloatRect *rect, void **pix, int *p) {

    if (rect == nullptr) {
        *pix = pixels;
    } else {
        *pix = (void *) (pixels + (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p != nullptr) {
        *p = pitch;
    }

    return 0;
}

void CTRTexture::unlock() {

    // tile buffer for 3ds...
    if (pixels != nullptr) {
        if (format == Format::RGB565) {
            upload();
        } else {
            uploadSoft();
        }
    }
}

void CTRTexture::setFilter(Filter filter) {

    GPU_TEXTURE_FILTER_PARAM param =
            filter == Filter::Point ? GPU_NEAREST : GPU_LINEAR;
    C3D_TexSetFilter(&tex, GPU_LINEAR, param);
}

void CTRTexture::upload() {

    GX_TRANSFER_FORMAT fmt = format == Format::RGB565 ? GX_TRANSFER_FMT_RGB565 : GX_TRANSFER_FMT_RGBA8;

    GSPGPU_FlushDataCache(pixels, (u32) tex.height * pitch);
    C3D_SyncDisplayTransfer(
            (u32 *) pixels,
            (u32) GX_BUFFER_DIM(tex.width, tex.height),
            (u32 *) tex.data,
            (u32) GX_BUFFER_DIM(tex.width, tex.height),
            (u32) TILE_FLAGS(fmt, fmt)
    );
}

void CTRTexture::uploadSoft() {

    if (pixels != nullptr) {
        int i, j, w = tex.width, h = tex.height;
        for (j = 0; j < h; j++) {
            for (i = 0; i < w; i++) {
                u32 coarse_y = static_cast<u32>(j & ~7);
                u32 dst_offset = get_morton_offset((u32) i, (u32) j, (u32) bpp) + coarse_y * pitch;
                u32 v = ((u32 *) pixels)[i + (h - 1 - j) * w];
                *(u32 *) ((u32) tex.data + dst_offset) = __builtin_bswap32(v); // RGBA8 -> ABGR8
            }
        }
    }
}

CTRTexture::~CTRTexture() {

    if (tex.data != nullptr) {
        C3D_TexDelete(&tex);
    }

    if (pixels != nullptr) {
        linearFree(pixels);
    }
}

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

    int w, h, n = 0;

    u8 *buf = stbi_load(path.c_str(), &w, &h, &n, 4);
    if (!buf) {
        printf("CTRTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    int pow2w = pow2(w), pow2h = pow2(h);
    pixels = (u8 *) linearAlloc((size_t) (pow2w * pow2h * 4));
    if (!pixels) {
        free(buf);
        return;
    }

    for (int i = 0; i < h; i++) {
        memcpy(pixels, buf, (size_t) w * 4);
        pixels += w * 4;
        buf += pow2w * 4;
    }

    free(buf);

    if (!C3D_TexInit(&tex, (u16) pow2w, (u16) pow2h, GPU_RGBA8)) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = nullptr;
        return;
    }

    setSize(w, h);
    setTexture(this);
    setTextureRect(IntRect(0, 0, tex.width, tex.height));
    pitch = w * bpp;
    upload();
    available = true;
}

CTRTexture::CTRTexture(const Vector2f &size, Format format) : Texture(size, format) {

    pixels = (u8 *) linearAlloc((size_t) getSize().y * pitch);
    if (!pixels) {
        return;
    }

    GPU_TEXCOLOR fmt = GPU_RGBA8;
    if (format == Format::RGB565) {
        fmt = GPU_RGB565;
    }

    bool res = C3D_TexInit(&tex, pow2((int) size.x), pow2((int) size.y), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = nullptr;
        return;
    }

    setSize(size);
    setTexture(this);
    setTextureRect(IntRect(0, 0, tex.width, tex.height));
    pitch = (int) size.x * bpp;
    available = true;
}

int CTRTexture::lock(FloatRect *rect, void **pix, int *p) {

    if (!rect) {
        *pix = pixels;
    } else {
        *pix = (void *) (pixels + (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void CTRTexture::unlock() {

    // tile buffer for 3ds...
    if (pixels) {
        upload();
    }
}

void CTRTexture::setFilter(Filter filter) {

    GPU_TEXTURE_FILTER_PARAM param =
            filter == Filter::Point ? GPU_NEAREST : GPU_LINEAR;
    C3D_TexSetFilter(&tex, GPU_LINEAR, param);
}

void CTRTexture::upload() {

    C3D_TexUpload(&tex, pixels);

    /*
    GSPGPU_FlushDataCache(pixels, (u32) getSize().y * pitch);
    GSPGPU_FlushDataCache(tex.data, tex.size);

    GX_TRANSFER_FORMAT fmt =
            format == Format::RGB565 ? GX_TRANSFER_FMT_RGB565 : GX_TRANSFER_FMT_RGBA8;

    C3D_SafeDisplayTransfer(
            (u32 *) pixels,
            (u32) GX_BUFFER_DIM((int) getSize().x, (int) getSize().y),
            (u32 *) tex.data,
            (u32) GX_BUFFER_DIM(tex.width, tex.height),
            (u32) TILE_FLAGS(fmt, fmt)
    );
    */
    /*
    C3D_SafeTextureCopy((u32 *) pixels,
                        (u32) GX_BUFFER_DIM((int) getSize().x, (int) getSize().y),
                        (u32 *) tex.data,
                        (u32) GX_BUFFER_DIM(tex.width, tex.height),
                        (u32) getSize().y * pitch,
                        (u32) TILE_FLAGS(fmt, fmt));
    */

    //gspWaitForPPF();
}

void CTRTexture::uploadSoft() {

    if (pixels && tex.data) {
        // TODO: add support for non-RGBA8 textures
        int i, j, w = (int) getSize().x, h = (int) getSize().y;
        for (j = 0; j < h; j++) {
            for (i = 0; i < w; i++) {
                u32 coarse_y = static_cast<u32>(j & ~7);
                u32 dst_offset = get_morton_offset(i, j, bpp) + coarse_y * tex.width * bpp;
                u32 v = ((u32 *) pixels)[i + (h - 1 - j) * w];
                *(u32 *) ((u32) tex.data + dst_offset) = __builtin_bswap32(v); /* RGBA8 -> ABGR8 */
            }
        }
    }
}

CTRTexture::~CTRTexture() {

    C3D_TexDelete(&tex);
    if (pixels != nullptr) {
        linearFree(pixels);
    }
}

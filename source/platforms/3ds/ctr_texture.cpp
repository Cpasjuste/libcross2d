//
// Created by cpasjuste on 01/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

#define linearAlloc malloc
#define linearFree free

#define TILE_FLAGS(inFmt, outFmt) \
    (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(inFmt) | GX_TRANSFER_OUT_FORMAT(outFmt) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

void *linear_realloc(void *mem, size_t size) {
    if (mem) {
        linearFree(mem);
    }
    mem = linearAlloc(size);
    return mem;
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
    available = createTexture() == 0;
}

CTRTexture::CTRTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {
    available = createTexture() == 0;
}

CTRTexture::CTRTexture(const Vector2i &size, Format format) : Texture(size, format) {
    available = createTexture() == 0;
}

int CTRTexture::createTexture() {
    if (!m_pixels) {
        printf("CTRTexture::createTexture: m_pixels is null\n");
        return -1;
    }

    m_tex = new C3D_Tex();

    if (m_format == Format::RGB565) {
        // TODO: test vram performance on unlock
        if (!C3D_TexInitVRAM(m_tex, (u16) m_tex_size.x, (u16) m_tex_size.y, GPU_RGB565)) {
            printf("CTRTexture::createTexture: C3D_TexInit failed\n");
            delete (m_tex);
            return -1;
        }
    } else {
        if (!C3D_TexInit(m_tex, (u16) m_tex_size.x, (u16) m_tex_size.y, GPU_RGBA8)) {
            printf("CTRTexture::createTexture: C3D_TexInit failed\n");
            delete (m_tex);
            return -1;
        }
    }

    unlock();

    return 0;
}

void CTRTexture::unlock(int rowLength) {
    // tile buffer for 3ds...
    if (m_tex && m_pixels) {
        if (m_format == Format::RGB565) {
            upload();
        } else {
            uploadSoft();
        }
    }
}

void CTRTexture::upload() {
    if (m_tex && m_pixels) {
        uint8_t *src = m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;
        GX_TRANSFER_FORMAT fmt = m_format == Format::RGB565 ? GX_TRANSFER_FMT_RGB565 : GX_TRANSFER_FMT_RGBA8;
        GSPGPU_FlushDataCache(m_pixels, (u32) m_tex->height * m_pitch);
        C3D_SyncDisplayTransfer(
                (u32 *) src,
                (u32) GX_BUFFER_DIM(m_unlock_rect.width, m_unlock_rect.height),
                (u32 *) m_tex->data,
                (u32) GX_BUFFER_DIM(m_tex_size.x, m_tex_size.y),
                (u32) TILE_FLAGS(fmt, fmt)
        );
    }
}

void CTRTexture::uploadSoft() {
    if (m_tex && m_pixels) {
        //printf("CTRTexture::uploadSoft(%p): rect: {%i, %i, %i, %i}, pixels: %p\n",
        //     this, m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height, m_pixels);

        // TODO: handle unlock rect...
#if 1
        int x, y, w = m_tex->width, h = m_tex->height;
        for (x = 0; x < h; x++) {
            for (y = 0; y < w; y++) {
                u32 coarse_y = static_cast<u32>(x & ~7);
                u32 dst_offset = get_morton_offset((u32) y, (u32) x, (u32) m_bpp) + coarse_y * m_pitch;
                u32 v = ((u32 *) m_pixels)[y + (h - 1 - x) * w];
                *(u32 *) ((u32) m_tex->data + dst_offset) = __builtin_bswap32(v); // RGBA8 -> ABGR8
            }
        }
#else
        //GSPGPU_FlushDataCache(m_pixels, (u32) m_tex_size.y * m_pitch);
        //GSPGPU_FlushDataCache(m_tex.data, (u32) m_tex.size);
        u32 *src = (u32 *) m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;

        for (int y = 0; y < m_unlock_rect.height; y++) {
            for (int x = 0; x < m_unlock_rect.width; x++) {
                u32 coarse_y = (u32) (m_unlock_rect.top + y) & ~7;
                u32 dst_offset =
                        get_morton_offset(m_unlock_rect.left + x, m_unlock_rect.top + y, m_bpp) + coarse_y * m_tex_size.x * m_bpp;
                u32 v = src[x + (m_unlock_rect.height - 1 - y) * m_unlock_rect.width];
                *(u32 *) ((u32) m_tex.data + dst_offset) = __builtin_bswap32(v); // RGBA8 -> ABGR8
            }
        }
#endif
    }
}

int CTRTexture::resize(const Vector2i &size, bool copyPixels) {
    printf("CTRTexture::resize: %ix%i > %ix%i\n",
           m_tex_size.x, m_tex_size.y, (int) size.x, (int) size.y);

    if (size == m_tex_size || size.x < m_tex_size.x || size.y < m_tex_size.y) {
        printf("CTRTexture::resize: invalid parameters\n");
        return -1;
    }

    auto tex_new = new C3D_Tex();

    if (m_format == Format::RGB565) {
        // TODO: test vram performance on unlock
        if (!C3D_TexInitVRAM(tex_new, (u16) size.x, (u16) size.y, GPU_RGB565)) {
            printf("CTRTexture::resize: C3D_TexInit failed\n");
            return -1;
        }
    } else {
        if (!C3D_TexInit(tex_new, (u16) size.x, (u16) size.y, GPU_RGBA8)) {
            printf("CTRTexture::resize: C3D_TexInit failed\n");
            return -1;
        }
    }

    auto *new_pixels = (uint8_t *) linearAlloc(size.x * size.y * m_bpp);

    if (copyPixels) {
        auto *src = m_pixels;
        auto *dst = new_pixels;
        int dst_pitch = size.x * m_bpp;
        for (int i = 0; i < m_tex_size.y; i++) {
            memcpy(dst, src, m_pitch);
            src += m_pitch;
            dst += dst_pitch;
        }
    } else {
        memset(new_pixels, 0, size.x * size.y * m_bpp);
    }

    // replace pixels
    linearFree(m_pixels);
    m_pixels = new_pixels;

    // update texture parameters
    C3D_TexDelete(m_tex);
    delete (m_tex);
    m_tex = tex_new;
    m_pitch = size.x * m_bpp;
    m_tex_size = m_image_size = size;
    setSize({(float) size.x, (float) size.y});
    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFilter(m_filter);

    // upload pixels
    uint8_t *p;
    lock(&p, nullptr, {0, 0, size.x, size.y});
    unlock();

    return 0;
}

void CTRTexture::setFilter(Filter filter) {
    GPU_TEXTURE_FILTER_PARAM param =
            filter == Filter::Point ? GPU_NEAREST : GPU_LINEAR;
    C3D_TexSetFilter(m_tex, param, param);
}

CTRTexture::~CTRTexture() {
    if (m_tex) {
        C3D_TexDelete(m_tex);
        delete (m_tex);
    }
}

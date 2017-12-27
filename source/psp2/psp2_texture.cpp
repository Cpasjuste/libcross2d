//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include <cstdio>
#include "c2d.h"

#ifdef __PSP2_DEBUG__

#include <psp2/kernel/clib.h>

#define printf sceClibPrintf
#endif

using namespace c2d;

PSP2Texture::PSP2Texture(const char *p) : Texture(p) {

    char *ext = strrchr(path, '.');
    if (ext) {
        if (strcasecmp(ext + 1, "jpg") == 0
            || strcasecmp(ext + 1, "jpeg") == 0) {
            tex = vita2d_load_JPEG_file(path);
        } else if (strcasecmp(ext + 1, "png") == 0) {
            tex = vita2d_load_PNG_file(path);
        } else if (strcasecmp(ext + 1, "bmp") == 0) {
            tex = vita2d_load_BMP_file(path);
        }
    }

    if (!tex) {
        printf("PSP2Texture: could not create texture from `%s`\n", path);
        return;
    }

    int w = vita2d_texture_get_width(tex);
    int h = vita2d_texture_get_height(tex);

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));

    pitch = vita2d_texture_get_stride(tex);

    available = true;
}

PSP2Texture::PSP2Texture(const Vector2f &size, int fmt) : Texture(size, fmt) {

    tex = vita2d_create_empty_texture_format(
            (unsigned int) size.x, (unsigned int) size.y,
            fmt == C2D_TEXTURE_FMT_RGBA8 ? SCE_GXM_TEXTURE_FORMAT_A8B8G8R8
                                         : SCE_GXM_TEXTURE_FORMAT_R5G6B5);

    if (!tex) {
        printf("PSP2Texture: couldn't create texture\n");
        return;
    }

    pitch = vita2d_texture_get_stride(tex);

    available = true;
}

int PSP2Texture::lock(FloatRect *rect, void **pixels, int *p) {

    if (!rect) {
        *pixels = vita2d_texture_get_datap(tex);
    } else {
        *pixels = (void *) ((uint8_t *) vita2d_texture_get_datap(tex) +
                            (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void PSP2Texture::setFiltering(int filter) {

    vita2d_texture_set_filters(tex,
                               SCE_GXM_TEXTURE_FILTER_POINT,
                               filter == C2D_TEXTURE_FILTER_POINT ?
                               SCE_GXM_TEXTURE_FILTER_POINT : SCE_GXM_TEXTURE_FILTER_LINEAR);
}

PSP2Texture::~PSP2Texture() {

    if (tex != nullptr) {
        vita2d_wait_rendering_done();
        vita2d_free_texture(tex);
        tex = nullptr;
    }
}

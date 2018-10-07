//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include <cstdio>
#include "cross2d/c2d.h"
#include "platforms/psp2/psp2_shaders.h"

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

    tex = vita2d_load_PNG_file(path);
    if (!tex) {
        printf("PSP2Texture: could not create texture from `%s`\n", path);
        return;
    }

    int w = vita2d_texture_get_width(tex);
    int h = vita2d_texture_get_height(tex);

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));

    pitch = vita2d_texture_get_stride(tex);
    setFiltering(filtering);
    setShader(0);

    available = true;
}

PSP2Texture::PSP2Texture(const Vector2f &size, int fmt) : Texture(size, fmt) {

    vita2d_texture_set_alloc_memblock_type(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW);
    tex = vita2d_create_empty_texture_format(
            (unsigned int) size.x, (unsigned int) size.y,
            fmt == C2D_TEXTURE_FMT_RGBA8 ? SCE_GXM_TEXTURE_FORMAT_A8B8G8R8
                                         : SCE_GXM_TEXTURE_FORMAT_R5G6B5);
    vita2d_texture_set_alloc_memblock_type(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW);

    if (!tex) {
        printf("PSP2Texture: couldn't create texture\n");
        return;
    }

    pitch = vita2d_texture_get_stride(tex);
    setFiltering(filtering);
    setShader(0);

    available = true;
}

int PSP2Texture::resize(const Vector2f &size, bool copyPixels) {

    if (size.x == getSize().x && size.y == getSize().y) {
        return 0;
    }

    vita2d_texture *tex_new = vita2d_create_empty_texture_format(
            (unsigned int) size.x, (unsigned int) size.y,
            format == C2D_TEXTURE_FMT_RGBA8 ? SCE_GXM_TEXTURE_FORMAT_A8B8G8R8
                                            : SCE_GXM_TEXTURE_FORMAT_R5G6B5);

    if (!tex_new) {
        printf("PSP2Texture: couldn't create texture\n");
        return -1;
    }

    if (copyPixels) {
        unsigned char *src = (unsigned char *) vita2d_texture_get_datap(tex);
        unsigned char *dst = (unsigned char *) vita2d_texture_get_datap(tex_new);
        int dst_pitch = vita2d_texture_get_stride(tex_new);
        Vector2i dst_size = Vector2i(
                std::min((int) getSize().x, (int) size.x),
                std::min((int) getSize().y, (int) size.y));

        for (int i = 0; i < dst_size.y; ++i) {
            memcpy(dst, src, (size_t) (dst_size.x * bpp));
            src += dst_size.x * bpp;
            dst += dst_pitch;
        }
    }

    vita2d_free_texture(tex);
    tex = tex_new;
    pitch = (int) (size.x * bpp);
    setSize(size);
    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFiltering(filtering);

    return 0;
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

    filtering = filter;
    vita2d_texture_set_filters(tex,
                               SCE_GXM_TEXTURE_FILTER_POINT,
                               filter == C2D_TEXTURE_FILTER_POINT ?
                               SCE_GXM_TEXTURE_FILTER_POINT : SCE_GXM_TEXTURE_FILTER_LINEAR);
}

void PSP2Texture::setShader(int shaderIndex) {

    PSP2ShaderList *shaderList = (PSP2ShaderList *) c2d_renderer->getShaderList();
    if (shaderIndex >= shaderList->getCount()) {
        return;
    }

    shader = shaderList->get(shaderIndex);
}

void PSP2Texture::applyShader() {

    vita2d_shader *v2d_shader = (vita2d_shader *) shader->data;

    sceGxmSetVertexProgram(vita2d_get_context(), v2d_shader->vertexProgram);
    sceGxmSetFragmentProgram(vita2d_get_context(), v2d_shader->fragmentProgram);

    // set shader params
    float *tex_size = (float *) vita2d_pool_memalign(2 * sizeof(float), sizeof(float));
    tex_size[0] = getSize().x;
    tex_size[1] = getSize().y;

    float *out_size = (float *) vita2d_pool_memalign(2 * sizeof(float), sizeof(float));
    out_size[0] = getSize().x * getScale().x;
    out_size[1] = getSize().y * getScale().y;

    if (v2d_shader->vertexInput.texture_size > 0) {
        PSP2ShaderList::setVertexUniform(v2d_shader->vertexInput.texture_size, tex_size, 2);
    }
    if (v2d_shader->vertexInput.output_size > 0) {
        PSP2ShaderList::setVertexUniform(v2d_shader->vertexInput.output_size, out_size, 2);
    }
    if (v2d_shader->vertexInput.video_size > 0) {
        PSP2ShaderList::setVertexUniform(v2d_shader->vertexInput.video_size, tex_size, 2);
    }
    if (v2d_shader->fragmentInput.texture_size > 0) {
        PSP2ShaderList::setFragmentUniform(v2d_shader->fragmentInput.texture_size, tex_size, 2);
    }
    if (v2d_shader->fragmentInput.output_size > 0) {
        PSP2ShaderList::setFragmentUniform(v2d_shader->fragmentInput.output_size, out_size, 2);
    }
    if (v2d_shader->fragmentInput.video_size > 0) {
        PSP2ShaderList::setFragmentUniform(v2d_shader->fragmentInput.video_size, tex_size, 2);
    }
}

PSP2Texture::~PSP2Texture() {

    if (tex != nullptr) {
        vita2d_wait_rendering_done();
        vita2d_free_texture(tex);
        tex = nullptr;
    }
}

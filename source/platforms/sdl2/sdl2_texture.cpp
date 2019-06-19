//
// Created by cpasjuste on 01/12/16.
//

#ifndef __SDL2_GL__

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

SDL2Texture::SDL2Texture(const std::string &path) : Texture(path) {

    int w, h, n = 0;
    unsigned char *pixels;

    pixels = stbi_load(path.c_str(), &w, &h, &n, 4);
    if (!pixels) {
        printf("SDL2Texture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    pitch = w * bpp;

    SDL_Surface *tmp =
            SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) c2d_renderer)->renderer, tmp);
    SDL_FreeSurface(tmp);
    free(pixels);
    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    setSize(w, h);
    setTexture(this, true);
    available = true;

    printf("SDL2Texture(%s): %p\n", path.c_str(), this);
}

SDL2Texture::SDL2Texture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    int w, h, n = 0;
    unsigned char *pixels;

    pixels = stbi_load_from_memory(buffer, bufferSize, &w, &h, &n, 4);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }

    pitch = w * bpp;

    SDL_Surface *tmp =
            SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) c2d_renderer)->renderer, tmp);
    SDL_FreeSurface(tmp);
    free(pixels);
    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    setSize(w, h);
    setTexture(this, true);
    available = true;

    printf("SDL2Texture(%p)\n", this);
}

SDL2Texture::SDL2Texture(const Vector2f &size, Format format) : Texture(size, format) {

    tex = SDL_CreateTexture(
            ((SDL2Renderer *) c2d_renderer)->renderer,
            format == Format::RGBA8 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING, (int) size.x, (int) size.y);

    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    setSize(size);
    setTexture(this, true);
    available = true;

    printf("SDL2Texture(%p)\n", this);
}

int SDL2Texture::lock(FloatRect *rect, void **pix, int *p) {

    int ret = 0;

    if (rect) {
        SDL_Rect r = {
                (Sint16) rect->left,
                (Sint16) rect->top,
                (Uint16) rect->width,
                (Uint16) rect->height
        };
        ret = SDL_LockTexture(tex, &r, pix, &pitch);
    } else {
        ret = SDL_LockTexture(tex, nullptr, pix, &pitch);
    }

    if (p) {
        *p = pitch;
    }

    return ret;
}

void SDL2Texture::unlock() {
    SDL_UnlockTexture(tex);
}

int SDL2Texture::save(const std::string &path) {

    int res;
    unsigned char pixels;
    int width = getTextureRect().width;
    int height = getTextureRect().height;

    SDL_LockTexture(tex, nullptr, (void **) &pixels, nullptr);

    if (!pixels) {
        return -1;
    }

    if (bpp == 2) {
        // convert rgb565 to bgr888
        auto *tmp = (unsigned char *) malloc((size_t) width * (size_t) height * 3);
        for (int i = 0; i < width * height; i++) {
            signed short nColour = ((signed short *) pixels)[i];
            *(tmp + i * 3 + 2) = (unsigned char) ((nColour & 0x1F) << 3);
            *(tmp + i * 3 + 2) |= *(tmp + 3 * i + 0) >> 5;
            *(tmp + i * 3 + 1) = (unsigned char) (((nColour >> 5) & 0x3F) << 2);
            *(tmp + i * 3 + 1) |= *(tmp + i * 3 + 1) >> 6;
            *(tmp + i * 3 + 0) = (unsigned char) (((nColour >> 11) & 0x1F) << 3);
            *(tmp + i * 3 + 0) |= *(tmp + i * 3 + 2) >> 5;
        }
        res = stbi_write_png(path.c_str(), width, height, 3, tmp, width * 3);
        free(tmp);
    } else {
        res = stbi_write_png(path.c_str(), width, height, 4, (const void *) pixels, width * 4);
    }

    return res;
}

void SDL2Texture::setFilter(Filter filter) {

#if 0
    if (!tex || filter == filtering) {
        return;
    }

    int access;
    SDL_QueryTexture(tex, nullptr, &access, nullptr, nullptr);
    if (access != SDL_TEXTUREACCESS_STREAMING) {
        printf("SDL2Texture::setFiltering: access != SDL_TEXTUREACCESS_STREAMING\n");
        return;
    }

    printf("SDL2Texture::setFiltering(%i)\n", filter);
    filtering = filter;

    if (filter == C2D_TEXTURE_FILTER_LINEAR) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    } else if (tex) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    }

    // SDL2 needs to re-create a texture to apply filtering...
    SDL_Texture *newTex = SDL_CreateTexture(
            ((SDL2Renderer *) c2d_renderer)->renderer,
            format == C2D_TEXTURE_FMT_RGBA8 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            (int) getSize().x, (int) getSize().y);

    void *src_pixels, *dst_pixels;
    int src_pitch, dst_pitch;

    SDL_LockTexture(tex, nullptr, &src_pixels, &src_pitch);
    SDL_LockTexture(newTex, nullptr, &dst_pixels, &dst_pitch);

    memcpy(dst_pixels, src_pixels, (size_t) pitch * (size_t) getSize().y);

    SDL_UnlockTexture(newTex);
    //SDL_UnlockTexture(tex);
    SDL_DestroyTexture(tex);
    tex = newTex;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
#endif
}

SDL2Texture::~SDL2Texture() {

    printf("~SDL2Texture(%p)\n", this);
    if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}

#endif // __SDL2_GL__

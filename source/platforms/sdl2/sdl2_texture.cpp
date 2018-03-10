//
// Created by cpasjuste on 01/12/16.
//

#ifndef __SDL2_GL__

#include <png.h>
#include "c2d.h"
#include "skeleton/lodepng.h"

using namespace c2d;

SDL2Texture::SDL2Texture(const char *path) : Texture(path) {

    unsigned int w, h, error = 0;

    unsigned char *pixels;
    error = lodepng_decode32_file(&pixels, &w, &h, path);
    if (error) {
        printf("SDL2Texture: couldn't create texture: %s\n", lodepng_error_text(error));
        return;
    }

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));
    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;
    pitch = (int) (getSize().x * bpp);

    SDL_Surface *tmp = SDL_CreateRGBSurfaceWithFormatFrom(
            pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
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

    available = true;

    printf("SDL2Texture(%p)\n", this);
}

SDL2Texture::SDL2Texture(const unsigned char *buffer, int bufferSize) {

    unsigned int w, h, error = 0;

    unsigned char *pixels;
    error = lodepng_decode32(&pixels, &w, &h, buffer, (size_t) bufferSize);
    if (error) {
        printf("SDL2Texture: couldn't create texture: %s\n", lodepng_error_text(error));
        return;
    }

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));
    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;
    pitch = (int) (getSize().x * bpp);

    SDL_Surface *tmp = SDL_CreateRGBSurfaceWithFormatFrom(
            pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
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

    available = true;

    printf("SDL2Texture(%p)\n", this);

}

SDL2Texture::SDL2Texture(const Vector2f &size, int format) : Texture(size, format) {

    tex = SDL_CreateTexture(
            ((SDL2Renderer *) c2d_renderer)->renderer,
            format == C2D_TEXTURE_FMT_RGBA8 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            (int) size.x, (int) size.y);

    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    available = true;

    printf("SDL2Texture(%p)\n", this);
}

int SDL2Texture::lock(FloatRect *rect, void **pix, int *p) {

    if (rect) {
        SDL_Rect r = {
                (Sint16) rect->left,
                (Sint16) rect->top,
                (Uint16) rect->width,
                (Uint16) rect->height
        };
        SDL_LockTexture(tex, &r, pix, &pitch);
    } else {
        SDL_LockTexture(tex, NULL, pix, &pitch);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void SDL2Texture::unlock() {

    SDL_UnlockTexture(tex);
}

void SDL2Texture::setFiltering(int filter) {

}

SDL2Texture::~SDL2Texture() {

    printf("~SDL2Texture(%p)\n", this);
    if (tex) {
        SDL_DestroyTexture(tex);
    }
}

#endif

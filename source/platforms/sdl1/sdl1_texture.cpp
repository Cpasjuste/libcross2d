//
// Created by cpasjuste on 01/12/16.
//

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

SDL1Texture::SDL1Texture(const std::string &path) : Texture(path) {

    int w, h, n = 0;

    pixels = stbi_load(path.c_str(), &w, &h, &n, 4);
    if (!pixels) {
        printf("SDL1Texture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    pitch = w * bpp;

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurfaceFrom(pixels, w, h, 32, pitch, rmask, gmask, bmask, amask);
    if (!surface) {
        stbi_image_free(pixels);
        printf("SDL1Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    setSize({w, h});
    setTexture(this, true);
    available = true;
    printf("SDL1Texture(%s): %p\n", path.c_str(), this);
}

SDL1Texture::SDL1Texture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    int w, h, n = 0;

    pixels = stbi_load_from_memory(buffer, bufferSize, &w, &h, &n, 4);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }

    pitch = w * bpp;

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurfaceFrom(pixels, w, h, 32, pitch, rmask, gmask, bmask, amask);
    if (!surface) {
        stbi_image_free(pixels);
        printf("SDL1Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    setSize({w, h});
    setTexture(this, true);
    available = true;

    printf("SDL1Texture(%p)\n", this);
}

SDL1Texture::SDL1Texture(const Vector2f &size, Format format) : Texture(size, format) {

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, (int) size.x, (int) size.y, 32, rmask, gmask, bmask, amask);
    if (!surface) {
        printf("SDL1Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    setSize(size);
    setTexture(this, true);
    available = true;

    printf("SDL1Texture(%p)\n", this);
}

int SDL1Texture::lock(FloatRect *rect, void **pix, int *p) {

    SDL_LockSurface(surface);
    if (!rect) {
        *pix = (unsigned char *) surface->pixels;
    } else {
        *pix = (void *) (((unsigned char *) surface->pixels) + (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void SDL1Texture::unlock() {
    SDL_UnlockSurface(surface);
}

int SDL1Texture::save(const std::string &path) {
    return 0;
}

void SDL1Texture::setFilter(Filter filter) {
}

SDL1Texture::~SDL1Texture() {

    printf("~SDL1Texture(%p)\n", this);
    if (pixels) {
        stbi_image_free(pixels);
    }
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

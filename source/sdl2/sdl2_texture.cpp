//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"
#include <SDL2/SDL_image.h>

using namespace c2d;

SDL2Texture::SDL2Texture(const char *p) : Texture(p) {

    SDL_Surface *temp = IMG_Load(p);
    if (temp == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s", path, SDL_GetError());
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) c2d_renderer)->sdl_renderer, temp);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
    } else {
        int w, h = 0;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        setSize(Vector2f(w, h));
        setTextureRect(IntRect(0, 0, w, h));
    }

    SDL_FreeSurface(temp);
}

SDL2Texture::SDL2Texture(const Vector2f &size) : Texture(size) {

    tex = SDL_CreateTexture(((SDL2Renderer *) c2d_renderer)->sdl_renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, (int) size.x, (int) size.y);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    }
}

int SDL2Texture::lock(const FloatRect &rect, void **data, int *pitch) {
    if (rect.top != 0 || rect.left != 0 || rect.width != 0 || rect.height != 0) {
        SDL_Rect r = {(int) rect.top, (int) rect.left, (int) rect.width, (int) rect.height};
        return SDL_LockTexture(tex, &r, data, pitch);
    } else {
        return SDL_LockTexture(tex, nullptr, data, pitch);
    }
}

void SDL2Texture::unlock() {
    SDL_UnlockTexture(tex);
}

void SDL2Texture::setFiltering(int filter) {

    char f[16];
    snprintf(f, 16, "%i\n", filter);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, f);
    // SDL2 only set filtering on texture creation :/
    if (tex != nullptr) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
    tex = SDL_CreateTexture(((SDL2Renderer *) c2d_renderer)->sdl_renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, (int) getSize().x, (int) getSize().y);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    }
}

SDL2Texture::~SDL2Texture() {
    if (tex != nullptr) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}

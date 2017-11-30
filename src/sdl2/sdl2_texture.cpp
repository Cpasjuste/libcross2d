//
// Created by cpasjuste on 01/12/16.
//

#include <C2D.h>
#include <SDL2/SDL_image.h>

SDL2Texture::SDL2Texture(Renderer *r, const char *p) : Texture(r, p) {

    SDL_Surface *temp = IMG_Load(path);
    if (temp == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s", path, SDL_GetError());
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) renderer)->sdl_renderer, temp);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
    } else {
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
    }

    SDL_FreeSurface(temp);
    available = true;
}

SDL2Texture::SDL2Texture(Renderer *r, int w, int h) : Texture(r, w, h) {

    tex = SDL_CreateTexture(((SDL2Renderer *) renderer)->sdl_renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    } else {
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        available = true;
    }
}

void SDL2Texture::Draw(int x, int y, int w, int h, float rotation) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopyEx(((SDL2Renderer *) renderer)->sdl_renderer, tex, NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
}

int SDL2Texture::Lock(const Rect &rect, void **data, int *pitch) {
    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        SDL_Rect r = {rect.x, rect.y, rect.w, rect.h};
        return SDL_LockTexture(tex, &r, data, pitch);
    } else {
        return SDL_LockTexture(tex, nullptr, data, pitch);
    }
}

void SDL2Texture::Unlock() {
    SDL_UnlockTexture(tex);
}

void SDL2Texture::SetFiltering(int filter) {
    char f[16];
    snprintf(f, 16, "%i\n", filter);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, f);
    // SDL2 only set filtering on texture creation :/
    if (tex != nullptr) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
    tex = SDL_CreateTexture(((SDL2Renderer *) renderer)->sdl_renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
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

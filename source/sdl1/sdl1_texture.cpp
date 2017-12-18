//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

SDL1Texture::SDL1Texture(Renderer *r, const char *p) : Texture(r, p) {

    surface = IMG_Load(path);
    if (surface == nullptr) {
        printf("Couldn't load %s: %s", path, SDL_GetError());
        return;
    }

    width = surface->w;
    height = surface->h;
    available = true;
}

SDL1Texture::SDL1Texture(Renderer *r, int w, int h) : Texture(r, w, h) {

    SDL_Surface *screen = ((SDL1Renderer *) renderer)->screen;
    surface = SDL_CreateRGBSurface(
            SDL_HWSURFACE | SDL_SRCCOLORKEY,
            width, height,
            screen->format->BitsPerPixel,
            screen->format->Rmask,
            screen->format->Gmask,
            screen->format->Bmask,
            screen->format->Amask);

    if (!surface) {
        printf("Couldn't create surface %s", SDL_GetError());
        return;
    }

    available = true;
}

void SDL1Texture::Draw(int x, int y, int w, int h, float rotation) {

    if (surface) {
        if (w == width && h == height && rotation == 0) {
            SDL_Rect dst = {(Sint16) x, (Sint16) y, (Uint16) w, (Uint16) h};
            SDL_BlitSurface(surface, NULL, ((SDL1Renderer *) renderer)->screen, &dst);
        } else {
            SDL_Surface *tmp = rotozoomSurfaceXY(
                    surface, rotation,
                    (float) w / (float) width,
                    (float) h / (float) height, 0);
            SDL_Rect dst = {(Sint16) x, (Sint16) y, (Uint16) w, (Uint16) h};
            SDL_BlitSurface(tmp, NULL, ((SDL1Renderer *) renderer)->screen, &dst);
            SDL_FreeSurface(tmp);
        }
    }
}

int SDL1Texture::Lock(const Rect &rect, void **data, int *pitch) {

    if (surface) {
        if (SDL_MUSTLOCK(surface)) {
            SDL_LockSurface(surface);
        }

        *data = surface->pixels;
        *pitch = surface->pitch;
    }
}

void SDL1Texture::Unlock() {

    if (surface) {
        if (SDL_MUSTLOCK(surface)) {
            SDL_UnlockSurface(surface);
        }
    }
}

void SDL1Texture::SetFiltering(int filter) {
}

SDL1Texture::~SDL1Texture() {
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

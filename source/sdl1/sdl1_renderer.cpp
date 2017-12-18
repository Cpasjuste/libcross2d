//
// Created by cpasjuste on 21/11/16.
//

#include "c2d.h"
#include <SDL/SDL_gfxPrimitives.h>

//////////
// INIT //
//////////
SDL1Renderer::SDL1Renderer(int w, int h) : Renderer(w, h) {

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    this->shaders = new Shaders("");
}
//////////
// INIT //
//////////

void SDL1Renderer::SetShader(int shader) {
    // TODO
}

void SDL1Renderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    lineRGBA(screen,
             (Sint16) x1, (Sint16) y1,
             (Sint16) x2, (Sint16) y2,
             c.r, c.g, c.b, c.a);
}

void SDL1Renderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    if (fill) {
        boxRGBA(screen,
                (Sint16) rect.x, (Sint16) rect.y,
                (Sint16) (rect.x + rect.w), (Sint16) (rect.y + rect.h),
                c.r, c.g, c.b, c.a);
    } else {
        rectangleRGBA(screen,
                      (Sint16) rect.x, (Sint16) rect.y,
                      (Sint16) (rect.x + rect.w), (Sint16) (rect.y + rect.h),
                      c.r, c.g, c.b, c.a);
    }
}

void SDL1Renderer::Clear() {
    Uint32 c = SDL_MapRGB(screen->format, color.r, color.g, color.b);
    SDL_FillRect(screen, NULL, c);
}

void SDL1Renderer::Flip() {

    SDL_Flip(screen);
}

void SDL1Renderer::Delay(unsigned int ms) {
    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {
    if (screen) {
        SDL_FreeSurface(screen);
    }
    SDL_Quit();
}

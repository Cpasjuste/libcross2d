//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include <SDL2/SDL.h>
#include "sdl2/sdl2_renderer.h"
#include "sdl2/sdl2_rectangle.h"

using namespace C2D;

SDL2Rectangle::SDL2Rectangle(
        int posX, int posY, int width, int height, float rotation, const Color &color)
        : Rectangle(posX, posY, width, height, rotation, color) {

    printf("SDL2Rectangle(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void SDL2Rectangle::Draw() {

    // draw
    SDL2Renderer *rdr = (SDL2Renderer *) renderer;
    printf("SDL2Rectangle(%p): Draw (renderer=%p)\n", this, rdr);

    SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
    if (color.a < 255) {
        SDL_SetRenderDrawBlendMode(rdr->sdl_renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(rdr->sdl_renderer, color.r, color.g, color.b, color.a);

    if (1) {
        SDL_RenderFillRect(rdr->sdl_renderer, &r);
    } else {
        SDL_RenderDrawRect(rdr->sdl_renderer, &r);
    }

    if (color.a < 255) {
        SDL_SetRenderDrawBlendMode(rdr->sdl_renderer, SDL_BLENDMODE_NONE);
    }

    SDL_SetRenderDrawColor(rdr->sdl_renderer, color.r, color.g, color.b, color.a);

    // call base function (draw childs)
    Rectangle::Draw();
}

SDL2Rectangle::~SDL2Rectangle() {
    printf("~SDL2Rectangle(%p)\n", this);
}

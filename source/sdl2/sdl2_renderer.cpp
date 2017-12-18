//
// Created by cpasjuste on 21/11/16.
//

#include "c2d.h"

using namespace c2d;

SDL2Renderer::SDL2Renderer(const Vector2f &size) : Renderer(size) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!getSize().x || !getSize().y) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            getSize().x, getSize().y, flags);

    if (window == nullptr) {
        window = SDL_CreateWindow(
                "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                getSize().x, getSize().y, 0);
        if (window == nullptr) {
            printf("Couldn't create window: %s\n", SDL_GetError());
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
            return;
        }
    }

    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == nullptr) {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return;
    }

    printf("SDL2Renderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

/*
void SDL2Renderer::DrawLine(Line *line) {

    if (!line || line->getVisibility() == C2D_VISIBILITY_HIDDEN) {
        return;
    }

    printf("SDL2Renderer(%p): DrawLine(%p)\n", this, line);

    // set color
    Color c = line->GetColor();
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);

    // draw with world/absolute positions (GetWorldRect)
    // x = x1, y = y1, w = x2; h = y2
    //Vec4 r = line->GetRect();
    std::vector<Vec2> bounds = line->GetLocalBounds();
    SDL_RenderDrawLine(sdl_renderer,
                       (int) bounds[0].x, (int) bounds[0].y,
                       (int) bounds[1].x, (int) bounds[1].y);

    // reset color
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
    }
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
}
*/

void SDL2Renderer::DrawRectangle(const FloatRect &rect, const Color &c) {

    //printf("SDL2Renderer(%p): DrawRectangle(%p)\n", this, rectangle);

    // set color
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);

    printf("SDL2Renderer(%p): DrawRectangle(): x:%i, y:%i, w:%i, h:%i\n",
           this,
           (int) rect.top,
           (int) rect.left,
           (int) rect.width,
           (int) rect.height);

    SDL_Rect r_sdl = {
            (int) rect.top,
            (int) rect.left,
            (int) rect.width,
            (int) rect.height};

    // if (rectangle->fill) {
    SDL_RenderFillRect(sdl_renderer, &r_sdl);
    // } else {
    //     SDL_RenderDrawRect(sdl_renderer, &r_sdl);
    // }

    // reset color
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
    }
    SDL_SetRenderDrawColor(sdl_renderer,
                           getFillColor().r,
                           getFillColor().g,
                           getFillColor().b,
                           getFillColor().a);
}

void SDL2Renderer::Flip() {

    printf("SDL2Renderer(%p): Flip\n", this);

    // Clear
    // clear screen
    SDL_SetRenderDrawColor(sdl_renderer,
                           getFillColor().r,
                           getFillColor().g,
                           getFillColor().b,
                           getFillColor().a);
    SDL_RenderClear(sdl_renderer);


    // call base class (draw childs)
    Renderer::Flip();

    // flip
    SDL_RenderPresent(sdl_renderer);
}

void SDL2Renderer::SetShader(int shader) {
    // TODO
}

void SDL2Renderer::Delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer\n");
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

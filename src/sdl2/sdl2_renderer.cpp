//
// Created by cpasjuste on 21/11/16.
//

#include <C2D.h>

//////////
// INIT //
//////////
SDL2Renderer::SDL2Renderer(int w, int h) : Renderer(w, h) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!w || !h) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
            "SDL2 GUI",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h, flags);

    if (window == nullptr) {
        printf("Couldn't create window: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == nullptr) {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return;
    }

    this->shaders = new Shaders("");
}
//////////
// INIT //
//////////

void SDL2Renderer::SetShader(int shader) {
    // TODO
}

void SDL2Renderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);

    SDL_RenderDrawLine(sdl_renderer, x1, y1, x2, y2);
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
    }

    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
}

void SDL2Renderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);

    if (fill) {
        SDL_RenderFillRect(sdl_renderer, &r);
    } else {
        SDL_RenderDrawRect(sdl_renderer, &r);
    }
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
    }

    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
}

void SDL2Renderer::Clip(const Rect &rect) {

    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
        SDL_RenderSetClipRect(sdl_renderer, &r);
    } else {
        SDL_RenderSetClipRect(sdl_renderer, NULL);
    }
}

void SDL2Renderer::Clear() {
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
}

void SDL2Renderer::Flip() {
    SDL_RenderPresent(sdl_renderer);
}

void SDL2Renderer::Delay(unsigned int ms) {
    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

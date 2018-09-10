//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GLES__

#include "c2d.h"

using namespace c2d;

static SDL_GLContext ctx;

SDL2GLESRenderer::SDL2GLESRenderer(const Vector2f &size) : GLESRenderer(size) {

    printf("SDL2GLESRenderer\n");

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!getSize().x || !getSize().y) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, flags);

    if (window == nullptr) {
        printf("Couldn't create window: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        printf("Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        return;
    }

    setSize(getSize());

    SDL_GL_SetSwapInterval(1);

    available = true;

    printf("SDL2GLESRenderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void SDL2GLESRenderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLESRenderer::flip();

        // flip TODO:
        // glFinish();
    }

    // flip
    SDL_GL_SwapWindow(window);
}

void SDL2GLESRenderer::delay(unsigned int ms) {

    SDL_Delay((Uint32) ms);
}

SDL2GLESRenderer::~SDL2GLESRenderer() {

    printf("~SDL2Renderer\n");
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif

//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GL__

#include "cross2d/c2d.h"

using namespace c2d;

SDL2Renderer::SDL2Renderer(const Vector2f &s) : GLRenderer(s) {

    printf("SDL2Renderer(GL)\n");

    SDL_ShowCursor(SDL_DISABLE);

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (s.x <= 0 || s.y <= 0) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

#ifdef __SDL2_GLES__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __SWITCH__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    printf("SDL_CreateWindow\n");
    window = SDL_CreateWindow(
            "CROSS2D_SDL2_GL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) s.x, (int) s.y, flags);
    if (window == nullptr) {
        printf("Couldn't SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    int x, y;
    SDL_GetWindowSize(window, &x, &y);
    m_size.x = x;
    m_size.y = y;

    printf("SDL_GL_CreateContext\n");
    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        printf("Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        return;
    }

    initGL();

    printf("GL vendor   : %s\n", glGetString(GL_VENDOR));
    printf("GL renderer : %s\n", glGetString(GL_RENDERER));
    printf("GL version  : %s\n", glGetString(GL_VERSION));
    printf("GL glsl     : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    available = true;

    printf("SDL2Renderer(GL)(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
}

void SDL2Renderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GLRenderer::flip(draw, inputs);

    // flip
    SDL_GL_SwapWindow(window);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL_Window *SDL2Renderer::getWindow() {
    return window;
}

SDL_GLContext SDL2Renderer::getContext() {
    return context;
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer(GL)\n");

    if (context) {
        SDL_GL_DeleteContext(context);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

#endif

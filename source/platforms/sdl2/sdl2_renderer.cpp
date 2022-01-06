//
// Created by cpasjuste on 21/11/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

static SDL_Window *window = nullptr;
static SDL_GLContext context = nullptr;

#ifdef __PSP2__
unsigned int sceLibcHeapSize = 2 * 1024 * 1024;
#endif

SDL2Renderer::SDL2Renderer(const Vector2f &s) : GLRenderer(s) {

    SDL_ShowCursor(SDL_DISABLE);

#ifdef __PS4__
    SDL_SetHint(SDL_HINT_PS4_PIGLET_MODULES_PATH, "/data/self/system/common/lib");
#endif

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (s.x <= 0 || s.y <= 0) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

#ifdef __GLES2__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __SWITCH__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif __GL1__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#elif __GL2__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetSwapInterval(1);

    window = SDL_CreateWindow(
            "CROSS2D_SDL2_GL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) s.x, (int) s.y, flags);
    if (window == nullptr) {
        printf("Couldn't SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        printf("Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        return;
    }

#if __GLAD__
    // amdgpu proprietary driver 19.30 and SDL2 getproc bug
    // it's seems safer to also use glad on linux (nintendo switch also use this)
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#elif __GLEW__
    glewInit();
#endif

    GLRenderer::initGL();

    // we need to delete gl context after all resources are freed by C2DObject destructor
    std::atexit(exitCallback);

    available = true;

    printf("SDLRenderer(SDL2)(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
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

void SDL2Renderer::exitCallback() {

    if (context != nullptr) {
        SDL_GL_DeleteContext(context);
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

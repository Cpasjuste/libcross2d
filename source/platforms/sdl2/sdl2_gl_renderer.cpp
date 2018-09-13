//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GL__

#include "c2d.h"

using namespace c2d;

static SDL_GLContext ctx;

SDL2Renderer::SDL2Renderer(const Vector2f &size) : GLRenderer(size) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!getSize().x || !getSize().y) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, flags);

    if (window == nullptr) {
        window = SDL_CreateWindow(
                "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                (int) getSize().x, (int) getSize().y, 0);
        if (window == nullptr) {
            printf("Couldn't create window: %s\n", SDL_GetError());
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
            return;
        }
    }

    ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        printf("Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetSwapInterval(1);

    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &minor);
    printf("OpenGL: %i.%i\n", major, minor);

    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(0));

    // vbo
    GL_CHECK(glGenBuffers(1, &vbo));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1024, nullptr, GL_STREAM_DRAW));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));

    shaderList = (ShaderList *) new GLShaderList();

    available = true;

    printf("SDL2Renderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void SDL2Renderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLRenderer::flip();
    }

    // flip
    // GL_CHECK(glFinish());
    SDL_GL_SwapWindow(window);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    GL_CHECK(glDeleteBuffers(1, &vbo));
    GL_CHECK(glDeleteVertexArrays(1, &vao));

    printf("~SDL2Renderer\n");
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif

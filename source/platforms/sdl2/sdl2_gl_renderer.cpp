//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GL__

#include "c2d.h"

using namespace c2d;

static SDL_GLContext ctx;

SDL2Renderer::SDL2Renderer(const Vector2f &size) : GLRenderer(size) {

    GLenum err;

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
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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
    glGenVertexArrays(1, &vao);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glGenVertexArrays: 0x%x\n", err);
        return;
    }
    glBindVertexArray(0);

    // vbo
    glGenBuffers(1, &vbo);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glGenBuffers: 0x%x\n", err);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glBindBuffer: 0x%x\n", err);
        return;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1024, nullptr, GL_STREAM_DRAW);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glBufferData: 0x%x\n", err);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glBindBuffer: 0x%x\n", err);
        return;
    }

    glDisable(GL_DEPTH_TEST);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glDisable(GL_DEPTH_TEST): 0x%x\n", err);
        return;
    }
    glDepthMask(GL_FALSE);
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glDepthMask(GL_FALSE): 0x%x\n", err);
        return;
    }

    shaderList = (ShaderList *) new GLShaderList("");

    available = true;

    printf("SDL2Renderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void SDL2Renderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLRenderer::flip();
    }

    // flip
    glFinish();
    SDL_GL_SwapWindow(window);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer\n");
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif

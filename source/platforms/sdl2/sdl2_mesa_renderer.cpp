//
// Created by cpasjuste on 21/11/16.
//

#ifndef __SDL2_GL__

#include <GL/gl.h>
#include <GL/osmesa.h>

#include "c2d.h"

using namespace c2d;

static OSMesaContext mesa_ctx = NULL;

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

    surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(surface);
    if (!renderer) {
        printf("Couldn't create software renderer: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return;
    }

    mesa_ctx = OSMesaCreateContextExt(OSMESA_BGRA, 0, 0, 0, NULL);
    /*
    switch (surface->format->format) {
        case SDL_PIXELFORMAT_RGBA8888:
            printf("OSMESA_RGBA\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_RGBA, 0, 0, 0, NULL);
            break;

        case SDL_PIXELFORMAT_BGRA8888:
            printf("OSMESA_BGRA\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_BGRA, 0, 0, 0, NULL);
            break;

        case SDL_PIXELFORMAT_ARGB8888:
            printf("OSMESA_ARGB\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_ARGB, 0, 0, 0, NULL);
            break;

        case SDL_PIXELFORMAT_RGB888:
            printf("OSMESA_RGB\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_RGB, 0, 0, 0, NULL);
            break;

        case SDL_PIXELFORMAT_BGR888:
            printf("OSMESA_BGR\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_BGR, 0, 0, 0, NULL);
            break;

        case SDL_PIXELFORMAT_RGB565:
            printf("OSMESA_RGB_565\n");
            mesa_ctx = OSMesaCreateContextExt(OSMESA_RGB_565, 0, 0, 0, NULL);
            break;

        default:
            break;
    }
    */

    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        available = false;
        return;
    }

    if (!OSMesaMakeCurrent(mesa_ctx, surface->pixels,
                           GL_UNSIGNED_BYTE, (GLsizei) getSize().x, (GLsizei) getSize().y)) {
        printf("OSMesaMakeCurrent (8 bits/channel) failed!\n");
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    OSMesaPixelStore(OSMESA_Y_UP, 0);

    available = true;

    printf("SDL2Renderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void SDL2Renderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw mesa buffer to screen)
    glFinish();

    // flip
    SDL_UpdateWindowSurface(window);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif

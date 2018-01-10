//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 with OSMesa (OpenGL software wrapper) for rendering
//

#ifndef __SDL1_GL__

#include <GL/gl.h>
#include <GL/osmesa.h>

#include "c2d.h"

using namespace c2d;

static OSMesaContext mesa_ctx;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : GLRenderer(size) {

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_SetVideoMode((int) getSize().x, (int) getSize().y,
                              32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    mesa_ctx = OSMesaCreateContextExt(OSMESA_BGRA, 0, 0, 0, NULL);
    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        SDL_FreeSurface(screen);
        available = false;
        return;
    }

    if (!OSMesaMakeCurrent(mesa_ctx, screen->pixels,
                           GL_UNSIGNED_BYTE, (GLsizei) getSize().x, (GLsizei) getSize().y)) {
        printf("OSMesaMakeCurrent (8 bits/channel) failed!\n");
        SDL_FreeSurface(screen);
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    OSMesaPixelStore(OSMESA_Y_UP, 0);

    available = true;

    this->shaders = new Shaders("");
}

void SDL1Renderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw mesa buffer to screen)
    glFinish();

    SDL_Flip(screen);
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    if (mesa_ctx) {
        OSMesaDestroyContext(mesa_ctx);
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    SDL_Quit();
}

#endif // __SDL1_GL__

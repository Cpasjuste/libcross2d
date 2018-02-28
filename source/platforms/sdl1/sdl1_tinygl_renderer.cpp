//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 with TinyGL (OpenGL software wrapper) for rendering
//

#ifdef __TINYGL__

#include "c2d.h"
#include "pTinyGL/pgl.h"

using namespace c2d;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : GLRenderer(size) {

    pglInit((int) size.x, (int) size.y);
    available = true;
}

void SDL1Renderer::flip(bool draw) {

    // call base class (draw childs)
    GLRenderer::flip(draw);

    // flip (draw buffer to screen)
    pglSwap();
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    pglClose();
}

#endif // __TINYGL__

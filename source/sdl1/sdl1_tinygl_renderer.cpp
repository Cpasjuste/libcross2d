//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 with OSMesa (OpenGL software wrapper) for rendering
//

#ifdef __TINYGL__

#include "c2d.h"
#include "GL/pgl.h"

using namespace c2d;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : GLRenderer(size) {

    pglInit((int) size.x, (int) size.y);

    this->shaders = new Shaders("");
    available = true;
}

void SDL1Renderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw buffer to screen)
    pglSwap();
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    pglClose();

    SDL_Quit();
}

#endif // __SDL1_GL__

//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 platform use TinyGL (OpenGL software wrapper) for rendering
//

#include "c2d.h"
#include "skeleton/TinyGL/GL/tinygl.h"
#include "../skeleton/TinyGL/zbuffer.h"

using namespace c2d;

static ZBuffer *gl_framebuffer = NULL;

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

    gl_framebuffer = ZB_open((int) getSize().x, (int) getSize().y,
                             ZB_MODE_RGBA, 0, 0, 0, 0);

    // map screen buffer to gl buffer for direct access
    gl_free(gl_framebuffer->pbuf);
    gl_framebuffer->pbuf = (PIXEL *) screen->pixels;

    glInit(gl_framebuffer);

    this->shaders = new Shaders("");
}

void SDL1Renderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    SDL_Flip(screen);
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    gl_framebuffer->frame_buffer_allocated = 0;
    ZB_close(gl_framebuffer);

    if (screen) {
        SDL_FreeSurface(screen);
    }

    SDL_Quit();
}

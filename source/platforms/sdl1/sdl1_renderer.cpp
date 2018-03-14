//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 OpenGL renderer
//

#ifdef __SDL1_GL__

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : GLRenderer(size) {

    printf("SDL1Renderer: using hardware rendering (opengl)\n");

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_SetVideoMode((int) getSize().x, (int) getSize().y,
                              32, SDL_OPENGL);
    if (!screen) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    available = true;
}

void SDL1Renderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLRenderer::flip();
    }

    SDL_GL_SwapBuffers();
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    if (screen) {
        SDL_FreeSurface(screen);
    }

    SDL_Quit();
}

#endif // __SDL1_GL__

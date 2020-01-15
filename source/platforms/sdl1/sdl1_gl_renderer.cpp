//
// Created by cpasjuste on 21/11/16.
//

#if defined(__SDL1__) && defined(__GL1__)

#include "cross2d/c2d.h"

using namespace c2d;

SDL1Renderer::SDL1Renderer(const Vector2f &s) : GL1Renderer(s) {

    printf("SDL1Renderer(GL)\n");

    SDL_ShowCursor(SDL_DISABLE);

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    m_size = s;
    SDL_SetVideoMode(m_size.x, m_size.y, 16, SDL_DOUBLEBUF | SDL_OPENGL);

    glInit();
    printf("GL vendor   : %s\n", glGetString(GL_VENDOR));
    printf("GL renderer : %s\n", glGetString(GL_RENDERER));
    printf("GL version  : %s\n", glGetString(GL_VERSION));

    available = true;

    printf("SDL1Renderer(GL)(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
}

void SDL1Renderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GL1Renderer::flip(draw, inputs);

    // flip
    glFlush();
    SDL_GL_SwapBuffers();
}

void SDL1Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    printf("~SDL1Renderer(GL)\n");
    SDL_Quit();
}

#endif

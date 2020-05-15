//
// Created by cpasjuste on 21/11/16.
//

#if defined(__SDL1__)

#include "cross2d/c2d.h"

using namespace c2d;

SDLRenderer::SDLRenderer(const Vector2f &s) : GLRenderer(s) {

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

#ifdef __WINDOWS__
    glewInit();
#elif __GLAD__
    // amdgpu proprietary driver 19.30 and SDL2 getproc bug
    // it's seems safer to also use glad on linux (nintendo switch also use this)
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif

    GLRenderer::initGL();

    available = true;

    printf("SDL1Renderer(GL)(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
}

void SDLRenderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GLRenderer::flip(draw, inputs);

    // flip
    glFlush();
    SDL_GL_SwapBuffers();
}

void SDLRenderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDLRenderer::~SDLRenderer() {

    printf("~SDL1Renderer(GL)\n");
    SDL_Quit();
}

#endif

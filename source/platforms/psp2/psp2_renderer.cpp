//
// Created by cpasjuste on 21/11/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

PSP2Renderer::PSP2Renderer(const Vector2f &s) : GLRenderer(s) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("PSP2Renderer: couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    vglInit(0x100000);
    vglWaitVblankStart(GL_TRUE);

    GLRenderer::initGL();

    available = true;

    printf("PSP2Renderer(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
}

void PSP2Renderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GLRenderer::flip(draw, inputs);

    // flip
    vglSwapBuffers(GL_FALSE);
}

void PSP2Renderer::delay(unsigned int ms) {
    const Uint32 max_delay = 0xffffffffUL / 1000;
    if (ms > max_delay) {
        ms = max_delay;
    }
    sceKernelDelayThreadCB(ms * 1000);
}

PSP2Renderer::~PSP2Renderer() {
    vglEnd();
}

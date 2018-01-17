//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 with OSMesa (OpenGL software wrapper) for rendering
//

#ifdef __TINYGL__

#include "c2d.h"
#include "zgl.h"

using namespace c2d;

//static ZBuffer *frameBuffer;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : GLRenderer(size) {

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_SetVideoMode((int) getSize().x, (int) getSize().y,
                              16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    /*
    frameBuffer = ZB_open((int) getSize().x, (int) getSize().y,
                          ZB_MODE_RGBA, 0, 0, 0, 0);
    gl_free(frameBuffer->pbuf);
    frameBuffer->pbuf = (PIXEL *) screen->pixels;
    glInit(frameBuffer);
    */

    SDL_PixelFormat *f = screen->format;
    Graphics::PixelFormat fmt = Graphics::PixelFormat(
            f->BytesPerPixel, 8 - f->Rloss, 8 - f->Gloss, 8 - f->Bloss, 0,
            f->Rshift, f->Gshift, f->Bshift, f->Ashift);

    Graphics::PixelBuffer screenBuffer =
            Graphics::PixelBuffer(fmt, (byte *) screen->pixels);

    TinyGL::FrameBuffer *fb = new TinyGL::FrameBuffer(
            (int) getSize().x, (int) getSize().y, screenBuffer);

    TinyGL::glInit(fb, 512);

    available = true;

    this->shaders = new Shaders("");
}

void SDL1Renderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw buffer to screen)
    TinyGL::tglPresentBuffer();

    SDL_Flip(screen);
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

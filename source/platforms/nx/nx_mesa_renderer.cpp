//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 with OSMesa (OpenGL software wrapper) for rendering
//

#ifdef __MESAGL__

#include <GL/gl.h>
#include <GL/osmesa.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL.h>

#ifdef NET_DEBUG
#include <nxlink_print.h>
#endif

#include "c2d.h"

using namespace c2d;

static OSMesaContext mesa_ctx = NULL;

NXMESARenderer::NXMESARenderer(const Vector2f &size) : GLRenderer(size) {

#ifdef NET_DEBUG
    nxlink_print_init();
#elif SVC_DEBUG
    consoleDebugInit(debugDevice_SVC);
    stdout = stderr;
#endif

    printf("NXMESARenderer: using software rendering (osmesa)\n");

    // we need an sdl window for input..
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, 0);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create sw renderer: %s, giving up...\n", SDL_GetError());
        return;
    }

    setSize(size.x, size.y);

    available = true;
}

void NXMESARenderer::setSize(const c2d::Vector2f &size) {

    setSize(size.x, size.y);
}

void NXMESARenderer::setSize(float width, float height) {

    gfxSetMode(GfxMode_LinearDouble);
    gfxConfigureResolution((s32) width, (s32) height);
    fb = gfxGetFramebuffer(NULL, NULL);

    if (mesa_ctx) {
        OSMesaDestroyContext(mesa_ctx);
    }

    mesa_ctx = OSMesaCreateContextExt(OSMESA_RGBA, 0, 0, 0, NULL);
    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        available = false;
        return;
    }

    if (!OSMesaMakeCurrent(mesa_ctx, fb,
                           GL_UNSIGNED_BYTE, (GLsizei) width, (GLsizei) height)) {
        printf("OSMesaMakeCurrent() failed!\n");
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    OSMesaPixelStore(OSMESA_Y_UP, 0);
    GLRenderer::setSize(width, height);
}

void NXMESARenderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLRenderer::flip();

        // flip (draw mesa buffer to screen)
        glFinish();
    }

    if (draw) {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    fb = gfxGetFramebuffer(NULL, NULL);
}

void NXMESARenderer::delay(unsigned int ms) {
    svcSleepThread((u64) 1000000 * ms);
}

NXMESARenderer::~NXMESARenderer() {

    if (mesa_ctx) {
        OSMesaDestroyContext(mesa_ctx);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();

#ifdef NET_DEBUG
    nxlink_print_exit();
#endif
}

#endif // __MESAGL__

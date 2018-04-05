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


#include <nxlink_print.h>

#include "c2d.h"

using namespace c2d;

static OSMesaContext mesa_ctx = NULL;

NXMESARenderer::NXMESARenderer(const Vector2f &size) : GLRenderer(size)
{
    printf("NXMESARenderer: using software rendering (osmesa)\n");

    // we need an sdl window for input..
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, 0);

    //gfxInitDefault();
    gfxSetMode(GfxMode_LinearDouble);

#ifdef NET_DEBUG
    nxlink_print_init();
#else
    consoleDebugInit(debugDevice_SVC);
    stdout = stderr;
#endif

    mesa_ctx = OSMesaCreateContextExt(OSMESA_RGBA, 0, 0, 0, NULL);
    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        available = false;
        return;
    }

    gfxConfigureResolution((s32) size.x, (s32) size.y);
    fb = gfxGetFramebuffer(NULL, NULL);

    if (!OSMesaMakeCurrent(mesa_ctx, fb,
                           GL_UNSIGNED_BYTE, (GLsizei) getSize().x, (GLsizei) getSize().y)) {
        printf("OSMesaMakeCurrent() failed!\n");
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    OSMesaPixelStore(OSMESA_Y_UP, 0);

    available = true;
}

void NXMESARenderer::flip(bool draw)
{
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

void NXMESARenderer::delay(unsigned int ms)
{
    svcSleepThread((u64) 1000000 * ms);
}

NXMESARenderer::~NXMESARenderer()
{
    if (mesa_ctx) {
        OSMesaDestroyContext(mesa_ctx);
    }

    if(window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();

#ifdef NET_DEBUG
    nxlink_print_exit();
#endif
}

#endif // __MESAGL__

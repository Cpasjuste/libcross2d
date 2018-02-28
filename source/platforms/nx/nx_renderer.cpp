//
// Created by cpasjuste on 21/11/16.
//

//
// NX with TinyGL (OpenGL software wrapper) rendering
//

#include <switch.h>
#include "c2d.h"
#include "pTinyGL/pgl.h"

using namespace c2d;

NXRenderer::NXRenderer(const Vector2f &size) : GLRenderer(size) {

#ifdef NET_DEBUG
    nx_net_init(NET_DEBUG_IP, 3333);
#elif SVC_DEBUG
    consoleDebugInit(debugDevice_SVC);
    stdout = stderr; // for yuzu
#endif

    // init video renderer
    pglInit((int) size.x, (int) size.y);
#ifdef SVC_DEBUG
    gfxSetMode(GfxMode_TiledSingle);
#endif

    available = true;
}

void NXRenderer::flip(bool draw) {

    // call base class (draw childs)
    GLRenderer::flip(draw);

    // flip (draw buffer to screen)
    if (draw) {
        pglSwap();
    } else {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }
}

void NXRenderer::delay(unsigned int ms) {

    svcSleepThread(1000000 * ms);
}

NXRenderer::~NXRenderer() {

    pglClose();
#ifdef NET_DEBUG
    nx_net_exit();
#endif
}

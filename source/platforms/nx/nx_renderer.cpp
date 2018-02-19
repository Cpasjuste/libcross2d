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

    pglInit((int) size.x, (int) size.y);

    //usbCommsInitialize();
    //consoleInit(NULL);
    //consoleDebugInit(debugDevice_SVC);

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
    //usbCommsExit();
}

void nx_printf(const char *str, ...) {

    va_list valist;
    va_start(valist, str);
    char buf[1024];
    size_t len = (size_t) vsnprintf(buf, 1024, str, valist);
    usbCommsWrite(buf, len);
    va_end(valist);
}

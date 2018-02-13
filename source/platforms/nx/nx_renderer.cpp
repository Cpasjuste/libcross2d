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

    consoleInit(NULL);
    consoleDebugInit(debugDevice_SVC);

    available = true;
}

void NXRenderer::flip() {

    appletMainLoop();

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw buffer to screen)
    pglSwap();
}

void NXRenderer::delay(unsigned int ms) {

    svcSleepThread(1000000 * ms);
}

NXRenderer::~NXRenderer() {

    pglClose();
}

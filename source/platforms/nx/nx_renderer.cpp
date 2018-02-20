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

#ifdef USB_DEBUG
bool usb = false;
#endif

NXRenderer::NXRenderer(const Vector2f &size) : GLRenderer(size) {

#ifdef USB_DEBUG
    Result ret = usbCommsInitialize();
    if (R_SUCCEEDED(ret)) {
        printf("usbCommsInitialize OK\n");
        usb = true;
    } else {
        printf("usbCommsInitialize FAIL\n");
    }
#elif NET_DEBUG
    nx_net_init("172.20.138.36", 3333);
    printf("NX READY >\n");
#elif SVC_DEBUG
    consoleInit(NULL);
    consoleDebugInit(debugDevice_SVC);
#endif

    pglInit((int) size.x, (int) size.y);
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
#ifdef USB_DEBUG
    if (usb) {
        usbCommsExit();
    }
#elif NET_DEBUG
    nx_net_exit();
#endif
}

#ifdef USB_DEBUG

void nx_printf(const char *str, ...) {

    if (usb) {
        va_list valist;
        va_start(valist, str);
        char buf[256];
        size_t len = (size_t) vsnprintf(buf, 256, str, valist);
        usbCommsWrite(buf, len);
        va_end(valist);
    }
}

#endif

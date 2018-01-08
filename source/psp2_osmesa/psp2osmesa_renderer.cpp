//
// Created by cpasjuste on 21/11/16.
//

#include <vita2d.h>
#include <psp2/kernel/threadmgr.h>

#include "c2d.h"
#include "GL/osmesa.h"

using namespace c2d;

static vita2d_texture *texture = NULL;
static OSMesaContext mesa_ctx = NULL;

PSP2OSMESARenderer::PSP2OSMESARenderer(const Vector2f &size) : GLRenderer(size) {

    const GLint z = 16, stencil = 0, accum = 0;
    GLint cBits;

    mesa_ctx = OSMesaCreateContextExt(OSMESA_RGBA, z, stencil, accum, NULL);
    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        return;
    }

    vita2d_init();
    texture = vita2d_create_empty_texture_format(
            (uint) getSize().x, (uint) getSize().y, (SceGxmTextureFormat) SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8);
    void *buffer = vita2d_texture_get_datap(texture);

    /* Bind the buffer to the context and make it current */
    if (!OSMesaMakeCurrent(mesa_ctx, buffer, GL_UNSIGNED_BYTE, w, h)) {
        printf("OSMesaMakeCurrent (8 bits/channel) failed!\n");
        vita2d_free_texture(texture);
        texture = NULL;
        OSMesaDestroyContext(mesa_ctx);
        mesa_ctx = NULL;
        return;
    }

    glGetIntegerv(GL_RED_BITS, &cBits);
    if (cBits != 8) {
        printf("Unable to create 8-bit/channel renderbuffer.\n");
        printf("May need to recompile Mesa with CHAN_BITS=16 or 32.\n");
        vita2d_free_texture(texture);
        texture = NULL;
        OSMesaDestroyContext(mesa_ctx);
        mesa_ctx = NULL;
        return;
    }

    glGetIntegerv(GL_GREEN_BITS, &cBits);
    glGetIntegerv(GL_BLUE_BITS, &cBits);
    glGetIntegerv(GL_ALPHA_BITS, &cBits);

    OSMesaColorClamp(GL_TRUE);
}

void PSP2OSMESARenderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    if (texture) {
        // flip (draw mesa buffer to screen)
        glFinish();

        vita2d_start_drawing();
        vita2d_clear_screen();
        vita2d_draw_texture_scale_rotate(texture, getSize().x / 2, getSize().y / 2, -1, 1, 180 * 0.0174532925f);
        vita2d_end_drawing();
        vita2d_wait_rendering_done();
        vita2d_swap_buffers();
    }
}

void PSP2OSMESARenderer::delay(unsigned int ms) {

    const Uint32 max_delay = 0xffffffffUL / 1000;
    if (ms > max_delay) {
        ms = max_delay;
    }
    sceKernelDelayThreadCB(ms * 1000);
}

PSP2OSMESARenderer::~PSP2OSMESARenderer() {

    vita2d_wait_rendering_done();
    vita2d_fini();

    if (mesa_ctx) {
        OSMesaDestroyContext(mesa_ctx);
    }

    if (texture) {
        vita2d_free_texture(texture);
    }
}

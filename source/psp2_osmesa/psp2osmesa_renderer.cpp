//
// Created by cpasjuste on 21/11/16.
//

#include <vita2d.h>
#include <psp2/kernel/threadmgr.h>

#include "c2d.h"
#include "GL/osmesa.h"

using namespace c2d;

static vita2d_texture *texture;
static OSMesaContext mesa_ctx = NULL;

PSP2OSMESARenderer::PSP2OSMESARenderer(const Vector2f &size) : GLRenderer(size) {

    const GLint z = 16, stencil = 0, accum = 0;
    GLint cBits;

    printf("OSMesaCreateContextExt\n");
    mesa_ctx = OSMesaCreateContextExt(OSMESA_ARGB, z, stencil, accum, NULL);
    if (!mesa_ctx) {
        printf("OSMesaCreateContextExt() failed!\n");
        available = false;
        return;
    }

    printf("vita2d_init\n");
    vita2d_init();
    printf("vita2d_create_empty_texture_format: %i %i\n", (uint) getSize().x, (uint) getSize().y);
    texture = vita2d_create_empty_texture_format(
            (uint) getSize().x, (uint) getSize().y, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8);
    if (texture == NULL) {
        printf("vita2d_create_empty_texture_format failed!\n");
        vita2d_fini();
        //OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    void *vbuffer = vita2d_texture_get_datap(texture);

    /*
    printf("OSMesaMakeCurrent\n");
    if (!OSMesaMakeCurrent(mesa_ctx, vita2d_texture_get_datap(texture),
                           GL_UNSIGNED_BYTE, (GLsizei) getSize().x, (GLsizei) getSize().y)) {
        printf("OSMesaMakeCurrent (8 bits/channel) failed!\n");
        vita2d_free_texture(texture);
        vita2d_fini();
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    printf("glGetIntegerv\n");
    glGetIntegerv(GL_RED_BITS, &cBits);
    if (cBits != 8) {
        printf("Unable to create 8-bit/channel renderbuffer.\n");
        printf("May need to recompile Mesa with CHAN_BITS=16 or 32.\n");
        vita2d_free_texture(texture);
        vita2d_fini();
        OSMesaDestroyContext(mesa_ctx);
        available = false;
        return;
    }

    glGetIntegerv(GL_GREEN_BITS, &cBits);
    glGetIntegerv(GL_BLUE_BITS, &cBits);
    glGetIntegerv(GL_ALPHA_BITS, &cBits);

    printf("OSMesaColorClamp\n");
    OSMesaColorClamp(GL_TRUE);
    */

    available = true;
}

void PSP2OSMESARenderer::flip() {

    if (available) {

        // call base class (draw childs)
        //GLRenderer::flip();

        // flip (draw mesa buffer to screen)
        //glFinish();

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

    if (available) {

        vita2d_wait_rendering_done();
        vita2d_fini();

        //if (mesa_ctx) {
        //    OSMesaDestroyContext(mesa_ctx);
       // }

        if (texture) {
            vita2d_free_texture(texture);
        }
    }
}

//
// Created by cpasjuste on 21/11/16.
//

#include <vita2d.h>
#include <psp2/kernel/threadmgr.h>

#include "c2d.h"
#include "skeleton/TinyGL/GL/tinygl.h"
#include "../skeleton/TinyGL/zbuffer.h"

using namespace c2d;

static ZBuffer *frameBuffer;
static vita2d_texture *texture;

PSP2GLRenderer::PSP2GLRenderer(const Vector2f &size) : GLRenderer(size) {

    vita2d_init();
    // vita2d_set_vblank_wait(0); // for speed testing

    texture = vita2d_create_empty_texture_format(
            (unsigned int) getSize().x, (unsigned int) getSize().y,
            SCE_GXM_TEXTURE_FORMAT_R5G6B5);

    frameBuffer = ZB_open((int) getSize().x, (int) getSize().y, ZB_MODE_5R6G5B, 0, 0, 0, 0);
    // map vita2d texture buffer to gl buffer for direct access
    gl_free(frameBuffer->pbuf);
    frameBuffer->pbuf = (PIXEL *) vita2d_texture_get_datap(texture);

    glInit(frameBuffer);
}

void PSP2GLRenderer::flip() {

    // call base class (draw childs)
    GLRenderer::flip();

    // flip (draw TinyGL buffer to screen
    vita2d_start_drawing();
    vita2d_draw_texture(texture, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
}

void PSP2GLRenderer::delay(unsigned int ms) {

    const Uint32 max_delay = 0xffffffffUL / 1000;
    if (ms > max_delay) {
        ms = max_delay;
    }
    sceKernelDelayThreadCB(ms * 1000);
}

PSP2GLRenderer::~PSP2GLRenderer() {

    frameBuffer->frame_buffer_allocated = 0;
    ZB_close(frameBuffer);
    vita2d_free_texture(texture);
    vita2d_fini();
}

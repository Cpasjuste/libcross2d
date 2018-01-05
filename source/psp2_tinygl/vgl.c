#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>

#include <psp2/power.h>
#include <vita2d.h>
#include "../skeleton/TinyGL/zbuffer.h"

typedef struct {
    ZBuffer *frameBuffer;
    vita2d_texture *texture;
    int w, h;
    unsigned int pitch;
    int mode;
} VGLScreen;

static VGLScreen *vScreen;

void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {

    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void vglInit(float width, float height) {

    // we want fastest speed as possible with this software implementation
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);

    vScreen = (VGLScreen *) malloc(sizeof(*vScreen));
    memset(vScreen, 0, sizeof(*vScreen));

    vita2d_init();
    // vita2d_set_vblank_wait(0); // for speed testing

    vScreen->texture = vita2d_create_empty_texture_format(
            (unsigned int) width, (unsigned int) height, SCE_GXM_TEXTURE_FORMAT_R5G6B5);
    vScreen->w = (int) width;
    vScreen->h = (int) height;
    vScreen->pitch = (unsigned int) vScreen->w * 2;
    vScreen->mode = ZB_MODE_5R6G5B;

    vScreen->frameBuffer = ZB_open(vScreen->w, vScreen->h, vScreen->mode, 0, 0, 0, 0);
    // map vita2d texture buffer to gl buffer for direct access
    gl_free(vScreen->frameBuffer->pbuf);
    vScreen->frameBuffer->pbuf = vita2d_texture_get_datap(vScreen->texture);

    glInit(vScreen->frameBuffer);
}

void vglSwap() {

    vita2d_start_drawing();
    vita2d_draw_texture(vScreen->texture, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
}

void vglClose() {

    vScreen->frameBuffer->frame_buffer_allocated = 0;
    ZB_close(vScreen->frameBuffer);
    vita2d_free_texture(vScreen->texture);
    free(vScreen);
    vita2d_fini();
}

void *vglGetFramebuffer() {
    return (void *) vScreen->frameBuffer->pbuf;
}

//
// Created by cpasjuste on 21/11/16.
//

#include <GL/gl.h>
#include <libvita2d/include/vita2d.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/clib.h>
#include "c2d.h"
#include "../skeleton/TinyGL/zbuffer.h"

using namespace c2d;

typedef struct {
    ZBuffer *frameBuffer;
    vita2d_texture *texture;
    //void *data;
    int w, h;
    unsigned int pitch;
    int mode;
} VGLScreen;

static VGLScreen *vScreen;


static void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {

    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


PSP2GLRenderer::PSP2GLRenderer(const Vector2f &size) : Renderer(size) {

    vScreen = (VGLScreen *) malloc(sizeof(*vScreen));
    memset(vScreen, 0, sizeof(*vScreen));

    vita2d_init();
    // vita2d_set_vblank_wait(0); // for speed testing
    //sceClibPrintf("PSP2GLRenderer: %f %f\n", getSize().x, getSize().y);

    vScreen->texture = vita2d_create_empty_texture_format(getSize().x, getSize().y, SCE_GXM_TEXTURE_FORMAT_R5G6B5);
    //vScreen->data = vita2d_texture_get_datap(vScreen->texture);
    vScreen->w = getSize().x;
    vScreen->h = getSize().y;
    vScreen->pitch = vScreen->w * 2;
    vScreen->mode = ZB_MODE_5R6G5B;

    vScreen->frameBuffer = ZB_open(vScreen->w, vScreen->h, vScreen->mode, 0, 0, 0, 0);
    // map vita2d texture buffer to gl buffer for direct access
    gl_free(vScreen->frameBuffer->pbuf);
    vScreen->frameBuffer->pbuf = (PIXEL *) vita2d_texture_get_datap(vScreen->texture);//(PIXEL *) vScreen->data;

    glInit(vScreen->frameBuffer);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // This Will Clear The Background Color To Black
    glClearDepth(1.0);                // Enables Clearing Of The Depth Buffer
    //glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                // Reset The Projection Matrix
    gluPerspective(45.0f, (GLfloat) getSize().x / (GLfloat) getSize().y, 0.1f,
                   100.0f);    // Calculate The Aspect Ratio Of The Window
    glMatrixMode(GL_MODELVIEW);
}

void PSP2GLRenderer::draw(const VertexArray &vertices,
                          const Transform &transform,
                          const Texture *texture) {

    /*
    //if (transform == Transform::Identity)
    //    glLoadIdentity();
    //else
    //    glLoadMatrixf(transform.getMatrix());
    //glPushMatrix();
    //glLoadMatrixf(transform.getMatrix());

    unsigned int count = vertices.getVertexCount();

    static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                   GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertices.getPrimitiveType()];

    glBegin(mode);

    for (unsigned int i = 0; i < count; i++) {

        Vector2f v = transform.transformPoint(vertices[i].position);

        glColor4f(1, 0, 0, 1);
        //glVertex2f(v.x, v.y);
        glVertex2f(vertices[i].position.x, vertices[i].position.y);

        //tiny3d_VertexPos(v.x, v.y, 0);
        //tiny3d_VertexColor(vertices[i].color.toRGBA());
        //if (tex) {
        //   tiny3d_VertexTexture(
        //           vertices[i].texCoords.x / texture->getSize().x,
        //           vertices[i].texCoords.y / texture->getSize().y);
        //}
    }

    glEnd();
  */

}

static float rtri = 0.0f;
static float rquad = 0.0f;

void PSP2GLRenderer::flip() {


    /*
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // call base class (draw childs)
    Renderer::flip();
    */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear The Screen And The Depth Buffer
    glLoadIdentity();                // Reset The View

    glTranslatef(-1.5f, 0.0f, -6.0f);        // Move Left 1.5 Units And Into The Screen 6.0

    glRotatef(rtri, 0.0f, 1.0f, 0.0f);        // Rotate The Triangle On The Y axis
    // draw a triangle (in smooth coloring mode)
    glBegin(GL_POLYGON);                // start drawing a polygon
    glColor3f(1.0f, 0.0f, 0.0f);            // Set The Color To Red
    glVertex3f(0.0f, 1.0f, 0.0f);        // Top
    glColor3f(0.0f, 1.0f, 0.0f);            // Set The Color To Green
    glVertex3f(1.0f, -1.0f, 0.0f);        // Bottom Right
    glColor3f(0.0f, 0.0f, 1.0f);            // Set The Color To Blue
    glVertex3f(-1.0f, -1.0f, 0.0f);        // Bottom Left
    glEnd();                    // we're done with the polygon (smooth color interpolation)

    glLoadIdentity();                // make sure we're no longer rotated.
    glTranslatef(1.5f, 0.0f, -6.0f);        // Move Right 3 Units, and back into the screen 6.0

    glRotatef(rquad, 1.0f, 0.0f, 0.0f);        // Rotate The Quad On The X axis
    // draw a square (quadrilateral)
    glColor3f(0.5f, 0.5f, 1.0f);            // set color to a blue shade.
    glBegin(GL_QUADS);                // start drawing a polygon (4 sided)
    glVertex3f(-1.0f, 1.0f, 0.0f);        // Top Left
    glVertex3f(1.0f, 1.0f, 0.0f);        // Top Right
    glVertex3f(1.0f, -1.0f, 0.0f);        // Bottom Right
    glVertex3f(-1.0f, -1.0f, 0.0f);        // Bottom Left
    glEnd();                    // done with the polygon

    rtri += 15.0f;                    // Increase The Rotation Variable For The Triangle
    rquad -= 15.0f;                    // Decrease The Rotation Variable For The Quad

    // flip
    vita2d_start_drawing();
    vita2d_draw_texture(vScreen->texture, 0, 0);
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

    vScreen->frameBuffer->frame_buffer_allocated = 0;
    ZB_close(vScreen->frameBuffer);
    vita2d_free_texture(vScreen->texture);
    free(vScreen);
    vita2d_fini();
}

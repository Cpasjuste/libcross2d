//
// Created by cpasjuste on 21/11/16.
//

#include "c2d.h"
#include "GL/gl.h"

using namespace c2d;

static bool gl_init = false;

GLRenderer::GLRenderer(const Vector2f &size) : Renderer(size) {

}

void GLRenderer::draw(const VertexArray &vertices,
                      const Transform &transform,
                      const Texture *texture) {

    unsigned int count = vertices.getVertexCount();

    glPushMatrix();

    if (transform == Transform::Identity) {
        glLoadIdentity();
    } else {
        glMultMatrixf(transform.getMatrix());
    }

    static const GLenum modes[] =
            {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
             GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertices.getPrimitiveType()];

    glBegin(mode);
    for (unsigned int i = 0; i < count; i++) {
        glColor4f(vertices[i].color.r / 255.0f,
                  vertices[i].color.g / 255.0f,
                  vertices[i].color.b / 255.0f,
                  vertices[i].color.a / 255.0f);
        glVertex2f(vertices[i].position.x, vertices[i].position.y);
    }
    glEnd();

    glPopMatrix();
}

void GLRenderer::flip() {

    if (!gl_init) {
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        //glShadeModel(GL_SMOOTH);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        gl_init = 1;
    }

    // clear screen
    glClearColor(getFillColor().r / 255.0f,
                 getFillColor().g / 255.0f,
                 getFillColor().b / 255.0f,
                 getFillColor().a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // call base class (draw childs)
    Renderer::flip();
}

GLRenderer::~GLRenderer() {

}

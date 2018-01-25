//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL__

#include "c2d.h"

#ifdef __TINYGL__

#include "GL/pgl.h"

#else

#include "GL/gl.h"

#endif

using namespace c2d;

static bool gl_init = false;

GLRenderer::GLRenderer(const Vector2f &size) : Renderer(size) {

}

void GLRenderer::draw(const VertexArray &vertices,
                      const Transform &transform,
                      const Texture *texture) {

    size_t count = vertices.getVertexCount();

    glPushMatrix();

    if (transform == Transform::Identity) {
        glLoadIdentity();
    } else {
        glLoadMatrixf(transform.getMatrix());
    }

    GLTexture *tex = ((GLTexture *) texture);
    if (tex && tex->available) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex->texID);
    }

    static const GLenum modes[] =
            {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
             GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertices.getPrimitiveType()];

    glBegin(mode);

    for (unsigned int i = 0; i < count; i++) {
        if (tex && tex->available) {
            glTexCoord2f(vertices[i].texCoords.x / texture->getSize().x,
                         vertices[i].texCoords.y / texture->getSize().y);
        }
        glColor4f(vertices[i].color.r / 255.0f,
                  vertices[i].color.g / 255.0f,
                  vertices[i].color.b / 255.0f,
                  vertices[i].color.a / 255.0f);
        glVertex2f(vertices[i].position.x, vertices[i].position.y);
    }

    glEnd();

    if (tex && tex->available) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

void GLRenderer::flip() {

    if (!gl_init) {

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);

        gl_init = true;
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

#endif // __GL__

//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GLES__

#include "c2d.h"

#include <GLES2/gl2.h>

using namespace c2d;

GLESRenderer::GLESRenderer(const Vector2f &size) : Renderer(size) {

    printf("GLESRenderer\n");

    setSize(size.x, size.y);
}

void GLESRenderer::draw(const VertexArray &vertices,
                        const Transform &transform,
                        const Texture *texture) {

    if (vertices.getVertexCount() == 0) {
        return;
    }



    /*
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

    if (tex || vertices[0].color.a < 255) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    if (tex || vertices[0].color.a < 255) {
        glDisable(GL_BLEND);
    }

    glPopMatrix();
    */
}

void GLESRenderer::setSize(const c2d::Vector2f &size) {

    setSize(size.x, size.y);

}

void GLESRenderer::setSize(float width, float height) {

    RectangleShape::setSize(width, height);

    //glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    glViewport(0, 0, static_cast<GLsizei>(getSize().x), static_cast<GLsizei>(getSize().y));
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    */
}

void GLESRenderer::flip(bool draw) {

    if (draw) {
        // clear screen
        glClearColor(getFillColor().r / 255.0f,
                     getFillColor().g / 255.0f,
                     getFillColor().b / 255.0f,
                     getFillColor().a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // call base class (draw childs)
    Renderer::flip(draw);
}

GLESRenderer::~GLESRenderer() {

}

#endif // __SDL2_GLES__

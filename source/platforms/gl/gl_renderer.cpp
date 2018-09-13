//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL__

#include "c2d.h"
#include <GL/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <err.h>

using namespace c2d;

GLRenderer::GLRenderer(const Vector2f &size) : Renderer(size) {

    setSize(size.x, size.y);
}

void GLRenderer::draw(const VertexArray &vertexArray,
                      const Transform &transform,
                      const Texture *texture) {

    GLenum err;
    GLShader *shader = (GLShader *) ((GLShaderList *) shaderList)->color->data;
    Vertex *vertices = vertexArray.getVertices().data();
    size_t count = vertexArray.getVertexCount();

    //printf("draw: mode=%i\n", vertexArray.getPrimitiveType());

    if (!texture) {

        glBindVertexArray(vao);
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glBindVertexArray: 0x%x\n", err);
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glBindBuffer: 0x%x\n", err);
            return;
        }

        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbo_offset, sizeof(Vertex) * count, vertices);
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glBufferSubData: 0x%x\n", err);
            return;
        }

        // set vertex position
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glVertexAttribPointer 0: 0x%x\n", err);
            return;
        }
        glEnableVertexAttribArray(0);

        // set vertex colors
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glVertexAttribPointer 1: 0x%x\n", err);
            return;
        }
        glEnableVertexAttribArray(1);

        // select shader
        glUseProgram(shader->program);
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glUseProgram: 0x%x\n", err);
            return;
        }

        // set projection matrix
        GLint loc_projMtx = glGetUniformLocation(shader->program, "projMtx");
        auto projMtx = glm::orthoLH(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
        glUniformMatrix4fv(loc_projMtx, 1, GL_FALSE, glm::value_ptr(projMtx));

        // set model matrix
        GLint loc_mdlvMtx = glGetUniformLocation(shader->program, "mdlvMtx");
        glUniformMatrix4fv(loc_mdlvMtx, 1, GL_FALSE, transform.getMatrix());


        if (texture || vertices[0].color.a < 255) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        // draw
        const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[vertexArray.getPrimitiveType()];

        glDrawArrays(mode, vbo_offset, (GLsizei) count);
        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glDrawArrays: 0x%x\n", err);
            return;
        }

        if (texture || vertices[0].color.a < 255) {
            glDisable(GL_BLEND);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    vbo_offset += count;

    /*
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
    GLenum mode = modes[vertexArray.getPrimitiveType()];

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
    */
}

void GLRenderer::flip(bool draw) {

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

    //printf("flip: buffer offset = %i\n", (int) (sizeof(Vertex) * offset));
    vbo_offset = 0;
}

GLRenderer::~GLRenderer() {

}

#endif // __GL__

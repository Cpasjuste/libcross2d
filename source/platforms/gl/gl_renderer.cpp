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

    Vertex *vertices = vertexArray.getVertices().data();
    size_t count = vertexArray.getVertexCount();
    GLTexture *tex = ((GLTexture *) texture);
    GLShader *shader = tex && tex->available ? (GLShader *) shaderList->get(0)->data :
                       (GLShader *) ((GLShaderList *) shaderList)->color->data;

    setFillColor(Color::Red);
    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbo_offset, sizeof(Vertex) * count, vertices));

    GL_CHECK(glUseProgram(shader->program));

    // set vertex position
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, position)));
    GL_CHECK(glEnableVertexAttribArray(0));

    // set vertex colors
    GL_CHECK(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, color)));
    GL_CHECK(glEnableVertexAttribArray(1));

    //printf("%f %f\n", texture->get, vertices->texCoords.y);

    if (tex && tex->available) {

        //GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex->texID));

        // set tex coords
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                       (void *) offsetof(Vertex, texCoords)));
        GL_CHECK(glEnableVertexAttribArray(2));

        // set texture unit
        //GLint loc_tex;
        //GL_CHECK(loc_tex = glGetUniformLocation(shader->program, "tex"));
        //GL_CHECK(glUniform1i(loc_tex, 0));

    }

    // set projection matrix
    GLint loc_projMtx;
    GL_CHECK(loc_projMtx = glGetUniformLocation(shader->program, "projMtx"));
    auto projMtx = glm::orthoLH(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
    GL_CHECK(glUniformMatrix4fv(loc_projMtx, 1, GL_FALSE, glm::value_ptr(projMtx)));

    // set model matrix
    GLint loc_mdlvMtx;
    GL_CHECK(loc_mdlvMtx = glGetUniformLocation(shader->program, "mdlvMtx"));
    GL_CHECK(glUniformMatrix4fv(loc_mdlvMtx, 1, GL_FALSE, transform.getMatrix()));

    // transform texture coordinates by the texture matrix
    GLint loc_texMtx;
    GL_CHECK(loc_texMtx = glGetUniformLocation(shader->program, "texMtx"));
    GL_CHECK(glUniformMatrix4fv(loc_texMtx, 1, GL_FALSE, texture->getTransform().getMatrix()));

    if (tex || vertices[0].color.a < 255) {
        GL_CHECK(glEnable(GL_BLEND));
        GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    // draw
    const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                            GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertexArray.getPrimitiveType()];
    GL_CHECK(glDrawArrays(mode, vbo_offset, (GLsizei) count));

    if (tex || vertices[0].color.a < 255) {
        GL_CHECK(glDisable(GL_BLEND));
    }

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));

    vbo_offset += count;
}

void GLRenderer::flip(bool draw) {

    if (draw) {

        // clear screen
        GL_CHECK(glClearColor(getFillColor().r / 255.0f,
                              getFillColor().g / 255.0f,
                              getFillColor().b / 255.0f,
                              getFillColor().a / 255.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    }

    // call base class (draw childs)
    Renderer::flip(draw);

    //printf("flip: buffer offset = %i\n", (int) (sizeof(Vertex) * offset));
    vbo_offset = 0;
}

GLRenderer::~GLRenderer() {

}

#ifndef NDEBUG

namespace c2d {
    void CheckOpenGLError(const char *stmt, const char *fname, int line) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
            abort();
        }
    }
}
#endif

#endif // __GL__

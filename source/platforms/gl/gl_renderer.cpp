//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL__

#include "c2d.h"
#include <GL/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace c2d;

GLRenderer::GLRenderer(const Vector2f &size) : Renderer(size) {

    setSize(size.x, size.y);
}

void GLRenderer::initGL() {

    // init shaders
    shaderList = (ShaderList *) new GLShaderList();

    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    // vbo
    GL_CHECK(glGenBuffers(1, &vbo));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTEX, nullptr, GL_STREAM_DRAW));
    //GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));
}

void GLRenderer::draw(const VertexArray &vertexArray,
                      const Transform &transform,
                      const Texture *texture) {

    Vertex *vertices = vertexArray.getVertices().data();
    size_t count = vertexArray.getVertexCount();
    GLTexture *tex = ((GLTexture *) texture);
    GLShader *shader = tex && tex->available ? (GLShader *) shaderList->get(0)->data :
                       (GLShader *) ((GLShaderList *) shaderList)->color->data;

    // bind vao/vbo
    if (vbo_offset + count >= MAX_VERTEX) {
        vbo_offset = 0;
    }

    //GL_CHECK(glBindVertexArray(vao));
    //GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbo_offset, sizeof(Vertex) * count, vertices));

    // set shader
    GL_CHECK(glUseProgram(shader->GetProgram()));

    // set vertex position
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, position)));
    GL_CHECK(glEnableVertexAttribArray(0));

    // set vertex colors
    GL_CHECK(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, color)));
    GL_CHECK(glEnableVertexAttribArray(1));

    if (tex && tex->available) {

        // bind texture
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex->texID));

        // set tex coords
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                       (void *) offsetof(Vertex, texCoords)));
        GL_CHECK(glEnableVertexAttribArray(2));

        // normalize texture coords
        GLfloat texMtx[16] = {1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f};
        texMtx[0] = 1.f / texture->getSize().x;
        texMtx[5] = 1.f / texture->getSize().y;
        //texMtx[10] = 1.f / texture->getSize().x;
        //texMtx[15] = 1.f / texture->getSize().y;

        shader->SetUniformMatrix("textureMatrix", texMtx);

    }

    // set projection matrix
    auto mtx = glm::orthoLH(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
    shader->SetUniformMatrix("projectionMatrix", glm::value_ptr(mtx));

    // set model view matrix
    shader->SetUniformMatrix("modelViewMatrix", transform.getMatrix());

    // enable blending if needed
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

    // disable vbo/vao
    //GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    //GL_CHECK(glBindVertexArray(0));

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
    // vbo_offset = 0;
}

GLRenderer::~GLRenderer() {

    printf("~GLRenderer\n");

    if (shaderList) {
        delete (shaderList);
        shaderList = nullptr;
    }

    if (vbo) {
        GL_CHECK(glDeleteBuffers(1, &vbo));
    }

    if (vao) {
        GL_CHECK(glDeleteVertexArrays(1, &vao));
    }
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

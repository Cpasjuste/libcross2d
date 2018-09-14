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

    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));

    // init shaders
    shaderList = (ShaderList *) new GLShaderList();
}

void GLRenderer::draw(VertexArray *vertexArray,
                      const Transform &transform,
                      const Texture *texture) {

    Vertex *vertices = vertexArray->getVertices().data();
    size_t vertexCount = vertexArray->getVertexCount();
    GLTexture *glTexture = ((GLTexture *) texture);
    GLShader *shader = glTexture && glTexture->available ? (GLShader *) shaderList->get(0)->data :
                       (GLShader *) ((GLShaderList *) shaderList)->color->data;

    // set shader
    GL_CHECK(glUseProgram(shader->GetProgram()));

    vertexArray->bindVbo();

    // set vertex position
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, position)));

    // set vertex colors
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, color)));

    if (glTexture && glTexture->available) {

        // bind texture
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, glTexture->texID));

        // set tex coords
        GL_CHECK(glEnableVertexAttribArray(2));
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                       (void *) offsetof(Vertex, texCoords)));

        // normalize texture coords
        GLfloat texMtx[16] = {1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f};
        texMtx[0] = 1.f / texture->getTextureRect().width;
        texMtx[5] = 1.f / texture->getTextureRect().height;
        shader->SetUniformMatrix("textureMatrix", texMtx);

    } else {
        GL_CHECK(glDisableVertexAttribArray(2));
    }

    // set projection matrix
    auto mtx = glm::orthoLH(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
    shader->SetUniformMatrix("projectionMatrix", glm::value_ptr(mtx));

    // set model view matrix
    shader->SetUniformMatrix("modelViewMatrix", transform.getMatrix());

    // enable blending if needed
    if (glTexture || vertices[0].color.a < 255) {
        GL_CHECK(glEnable(GL_BLEND));
        GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    // draw
    const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                            GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertexArray->getPrimitiveType()];
    GL_CHECK(glDrawArrays(mode, 0, (GLsizei) vertexCount));

    GL_CHECK(glDisableVertexAttribArray(0));
    GL_CHECK(glDisableVertexAttribArray(1));

    vertexArray->unbindVbo();

    if (glTexture || vertices[0].color.a < 255) {
        GL_CHECK(glDisable(GL_BLEND));
    }
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
}

GLRenderer::~GLRenderer() {

    printf("~GLRenderer\n");

    if (shaderList) {
        delete (shaderList);
        shaderList = nullptr;
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

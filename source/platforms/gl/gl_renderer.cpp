//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL__

#define GLM_FORCE_PURE

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cross2d/c2d.h"

using namespace c2d;

GLRenderer::GLRenderer(const Vector2f &size) : Renderer(size) {

    printf("GLRenderer\n");
}

void GLRenderer::initGL() {

#ifdef __SWITCH__
    gladLoadGL();
#elif __WINDOWS__
    glewInit();
#endif

    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));

    // enable position and color array by default
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glEnableVertexAttribArray(1));

    // init shaders
    shaderList = (ShaderList *) new GLShaderList();
}

void GLRenderer::draw(VertexArray *vertexArray,
                      const Transform &transform,
                      const Texture *texture) {

    Vertex *vertices;
    size_t vertexCount;
    GLTexture *glTexture;
    GLShader *shader;

    if (!vertexArray || vertexArray->getVertexCount() < 1) {
        //printf("gl_render::draw: no vertices\n");
        return;
    }

    vertices = vertexArray->getVertices()->data();
    vertexCount = vertexArray->getVertexCount();
    glTexture = ((GLTexture *) texture);
    shader = glTexture && glTexture->available ? (GLShader *) shaderList->get(0)->data :
             (GLShader *) ((GLShaderList *) shaderList)->color->data;
    if (glTexture && glTexture->shader) {
        shader = (GLShader *) glTexture->shader->data;
    }

    // bind object vao
    vertexArray->bind();

    // set shader
    GL_CHECK(glUseProgram(shader->GetProgram()));

    // set vertex position
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void *) offsetof(Vertex, position)));

    // set vertex colors
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

        // set retroarch shader params
        shader->SetUniform("InputSize", texture->getTextureRect().width, texture->getTextureRect().height);
        shader->SetUniform("TextureSize", texture->getTextureRect().width, texture->getTextureRect().height);
        // TODO: i don't get this..
        Vector2f outSize = getSize(); // SCALE_TYPE_SCREEN
        if (shader->getScaleType() == GLShader::SCALE_TYPE_VIEWPORT) {
            outSize = Vector2f{texture->getGlobalBounds().width, texture->getGlobalBounds().height};
        } else if (shader->getScaleType() == GLShader::SCALE_TYPE_SOURCE) {
            outSize = Vector2f{texture->getTextureRect().width, texture->getTextureRect().height};
        }
        shader->SetUniform("OutputSize", outSize);
        /*
        if (glTexture->shader) {
            printf("tex: %i x %i, out: %f x %f\n", texture->getTextureRect().width, texture->getTextureRect().height,
                   texture->getGlobalBounds().width, texture->getGlobalBounds().height);
        }
        */
    } else {
        GL_CHECK(glDisableVertexAttribArray(2));
    }

    //auto pMtx = glm::orthoLH(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
    //auto mMtx = glm::make_mat4(transform.getMatrix());
    //auto pmMtx = glm::matrixCompMult(pMtx, mMtx);
    //shader->SetUniformMatrix("MVPMatrix", glm::value_ptr(pmMtx));

    // set projection matrix
    int w, h;
    SDL_Window *window = ((SDL2Renderer *) this)->getWindow();
    SDL_GL_GetDrawableSize(window, &w, &h);
    auto mtx = glm::orthoLH(0.0f, (float) w, (float) h, 0.0f, 0.0f, 1.0f);

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

    if (glTexture || vertices[0].color.a < 255) {
        GL_CHECK(glDisable(GL_BLEND));
    }

    // unbind object vao
    vertexArray->unbind();
}

void GLRenderer::clear() {

    // clear screen
    GL_CHECK(glClearColor(m_clearColor.r / 255.0f,
                          m_clearColor.g / 255.0f,
                          m_clearColor.b / 255.0f,
                          m_clearColor.a / 255.0f));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::flip(bool draw) {

    if (draw) {
        clear();
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

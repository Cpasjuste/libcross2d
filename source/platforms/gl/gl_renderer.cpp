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

#ifndef __SDL2_GLES__
    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
#endif
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));

    // init shaders
    shaderList = (ShaderList *) new GLShaderList();
}

void GLRenderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) {

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
    glTexture = texture ? ((GLTexture *) texture) : nullptr;
    shader = glTexture && glTexture->available ? (GLShader *) shaderList->get(0)->data :
             (GLShader *) ((GLShaderList *) shaderList)->color->data;
    if (glTexture && glTexture->shader) {
        shader = (GLShader *) glTexture->shader->data;
    }

    // set shader
    GL_CHECK(glUseProgram(shader->GetProgram()));
#ifndef __SDL2_GLES__
    // bind vao
    GL_CHECK(glBindVertexArray(vao));
#endif
    // bind vbo
    vertexArray->bind();

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
        texMtx[0] = 1.f / texture->getSize().x;
        texMtx[5] = 1.f / texture->getSize().y;
        shader->SetUniformMatrix("textureMatrix", texMtx);

        // set retroarch shader params
        Vector2f inputSize = {texture->getTextureRect().width, texture->getTextureRect().height};
        Vector2f textureSize = {texture->getSize().x, texture->getSize().y};
        Vector2f outputSize = getSize(); // SCALE_TYPE_SCREEN
        if (shader->getScaleType() == GLShader::SCALE_TYPE_VIEWPORT) {
            outputSize = Vector2f{texture->getGlobalBounds().width, texture->getGlobalBounds().height};
        } else if (shader->getScaleType() == GLShader::SCALE_TYPE_SOURCE) {
            outputSize = Vector2f{(float) texture->getTextureRect().width * texture->getScale().x,
                                  (float) texture->getTextureRect().height * texture->getScale().y};
        }
        shader->SetUniform("InputSize", inputSize);
        shader->SetUniform("TextureSize", textureSize);
        shader->SetUniform("OutputSize", outputSize);
#if 0
        if (glTexture->shader) {
            printf("inputSize: %ix%i, textureSize: %ix%i, outputSize: %ix%i\n",
                   (int) inputSize.x, (int) inputSize.y,
                   (int) textureSize.x, (int) textureSize.y,
                   (int) outputSize.x, (int) outputSize.y);
        }
#endif
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

    GL_CHECK(glDisableVertexAttribArray(0));
    GL_CHECK(glDisableVertexAttribArray(1));

    if (glTexture || vertices[0].color.a < 255) {
        GL_CHECK(glDisable(GL_BLEND));
        if (glTexture && glTexture->available) {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
            GL_CHECK(glDisableVertexAttribArray(2));
        }
    }

    // unbind object vbo
    vertexArray->unbind();
#ifndef __SDL2_GLES__
    // unbind object vao
    glBindVertexArray(0);
#endif

    GL_CHECK(glUseProgram(0));
}

void GLRenderer::clear() {

    // clear screen
    GL_CHECK(glClearColor(m_clearColor.r / 255.0f,
                          m_clearColor.g / 255.0f,
                          m_clearColor.b / 255.0f,
                          m_clearColor.a / 255.0f));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::flip(bool draw, bool inputs) {

    process_inputs = inputs;
    Renderer::onUpdate();

    if (draw) {
        clear();
    }

    // call base class (draw childs)
    Renderer::flip(draw, inputs);
}

GLRenderer::~GLRenderer() {

    printf("~GLRenderer\n");

    if (shaderList) {
        delete (shaderList);
        shaderList = nullptr;
    }

#ifndef __SDL2_GLES__
    if (glIsVertexArray(vao)) {
        GL_CHECK(glDeleteVertexArrays(1, &vao));
    }
#endif
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

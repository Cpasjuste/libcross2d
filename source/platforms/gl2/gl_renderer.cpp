//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL2__

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

#if defined(__PLATFORM_LINUX__) || defined(__SWITCH__)
    // amdgpu proprietary driver 19.30 and SDL2 getproc bug
    // it's seems safer to also use glad on linux
    gladLoadGL();
#elif __WINDOWS__
    glewInit();
#endif

#ifndef __GLES2__
    // vao
    GL_CHECK(glGenVertexArrays(1, &vao));
#endif
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glDepthMask(GL_FALSE));

    // init shaders
    shaderList = (ShaderList *) new GLShaderList();
}

void GLRenderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture, Sprite *sprite) {

    Vertex *vertices;
    size_t vertexCount;
    GLTexture *glTexture;
    GLShader *shader;
    Vector2f inputSize, textureSize, outputSize;

    if (vertexArray == nullptr || vertexArray->getVertexCount() < 1) {
        //printf("gl_render::draw: no vertices\n");
        return;
    }

    vertices = vertexArray->getVertices()->data();
    vertexCount = vertexArray->getVertexCount();
    glTexture = sprite != nullptr ? (GLTexture *) sprite->getTexture() : (GLTexture *) texture;
    shader = glTexture != nullptr && glTexture->available ? (GLShader *) shaderList->get(0)->data :
             (GLShader *) ((GLShaderList *) shaderList)->color->data;
    if (glTexture != nullptr && glTexture->shader != nullptr) {
        shader = (GLShader *) glTexture->shader->data;
    }

    // set shader
    GL_CHECK(glUseProgram(shader->GetProgram()));
#ifndef __GLES2__
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

    if (glTexture != nullptr && glTexture->available) {
        // bind texture
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, glTexture->texID));
        // set tex coords
        GL_CHECK(glEnableVertexAttribArray(2));
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                       (void *) offsetof(Vertex, texCoords)));
        // set retroarch shader params
        if (sprite != nullptr) {
            inputSize = {sprite->getTextureRect().width, sprite->getTextureRect().height};
            textureSize = {sprite->getTexture()->getSize().x, sprite->getTexture()->getSize().y};
            outputSize = {inputSize.x * sprite->getScale().x, inputSize.y * sprite->getScale().y};
        } else {
            inputSize = {texture->getTextureRect().width, texture->getTextureRect().height};
            textureSize = {texture->getSize().x, texture->getSize().y};
            outputSize = {inputSize.x * texture->getScale().x, inputSize.y * texture->getScale().y};
        }
        shader->SetUniform("InputSize", inputSize);
        shader->SetUniform("TextureSize", textureSize);
        shader->SetUniform("OutputSize", outputSize);
#if 0
        if (glTexture->shader != nullptr) {
            printf("inputSize: %ix%i, textureSize: %ix%i, outputSize: %ix%i\n",
                   (int) inputSize.x, (int) inputSize.y,
                   (int) textureSize.x, (int) textureSize.y,
                   (int) outputSize.x, (int) outputSize.y);
        }
#endif
    }

    // projection
    int w, h;
    SDL_Window *window = ((SDL2Renderer *) this)->getWindow();
    SDL_GL_GetDrawableSize(window, &w, &h);
    auto projectionMatrix = glm::orthoLH(0.0f, (float) w, (float) h, 0.0f, 0.0f, 1.0f);
    // view
    auto viewMatrix = glm::make_mat4(transform.getMatrix());
    // mpv
    auto mpvMatrix = projectionMatrix * viewMatrix;
    // set mpv matrix uniform
    shader->SetUniformMatrix("MVPMatrix", glm::value_ptr(mpvMatrix));

    // enable blending if needed
    if (glTexture != nullptr || vertices[0].color.a < 255) {
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

    if (glTexture != nullptr || vertices[0].color.a < 255) {
        GL_CHECK(glDisable(GL_BLEND));
        if (glTexture != nullptr && glTexture->available) {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
            GL_CHECK(glDisableVertexAttribArray(2));
        }
    }

    // unbind object vbo
    vertexArray->unbind();
#ifndef __GLES2__
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

    // call base class (draw childs)
    Renderer::flip(draw, inputs);
}

GLRenderer::~GLRenderer() {

    printf("~GLRenderer\n");

#ifndef __GLES2__
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

#endif // __GL2__

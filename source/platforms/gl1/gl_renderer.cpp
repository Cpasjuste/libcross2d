//
// Created by cpasjuste on 21/11/16.
//

#ifdef __GL1__

//#define BLEND_TEST

#include "cross2d/c2d.h"

using namespace c2d;

GL1Renderer::GL1Renderer(const Vector2f &size) : Renderer(size) {
    printf("GLRenderer\n");
}

void GL1Renderer::glInit() {

#if defined(__PLATFORM_LINUX__) || defined(__SWITCH__)
    // amdgpu proprietary driver 19.30 and SDL2 getproc bug
    // it's seems safer to also use glad on linux
    gladLoadGL();
#elif __WINDOWS__
    glewInit();
#endif

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

#ifndef BLEND_TEST
    // GLdc doesn't like mixing blend and non blend textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, getSize().x, getSize().y, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GL1Renderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture, Sprite *sprite) {

    Vertex *vertices;
    size_t vertexCount;

    if (vertexArray == nullptr || vertexArray->getVertexCount() < 1) {
        //printf("GL1Renderer::draw: no vertices\n");
        return;
    }

    vertices = vertexArray->getVertices()->data();
    vertexCount = vertexArray->getVertexCount();

    if (transform == Transform::Identity) {
        glLoadIdentity();
    } else {
        glLoadMatrixf(transform.getMatrix());
    }

    GLTexture *tex = sprite != nullptr ? (GLTexture *) sprite->getTexture() : (GLTexture *) texture;
    if (tex != nullptr && tex->available) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex->texID);
#if BLEND_TEST
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    }
#if BLEND_TEST
    else if (vertices[0].color.a < 255) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
#endif

    const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                            GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[vertexArray->getPrimitiveType()];

    glBegin(mode);

    for (unsigned int i = 0; i < vertexCount; i++) {
        if (tex != nullptr && tex->available) {
            glTexCoord2f(vertices[i].texCoords.x, vertices[i].texCoords.y);
        }
        glColor4f((float) vertices[i].color.r / 255.0f,
                  (float) vertices[i].color.g / 255.0f,
                  (float) vertices[i].color.b / 255.0f,
                  (float) vertices[i].color.a / 255.0f);
        glVertex2f(vertices[i].position.x, vertices[i].position.y);
    }

    glEnd();

    if (tex != nullptr && tex->available) {
        glBindTexture(GL_TEXTURE_2D, 0);
#if BLEND_TEST
        glDisable(GL_BLEND);
#endif
    }
#if BLEND_TEST
    else if (vertices[0].color.a < 255) {
        glDisable(GL_BLEND);
    }
#endif
}

void GL1Renderer::clear() {

    glClearColor((float) m_clearColor.r / 255.0f,
                 (float) m_clearColor.g / 255.0f,
                 (float) m_clearColor.b / 255.0f,
                 (float) m_clearColor.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GL1Renderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    Renderer::flip(draw, inputs);
}

GL1Renderer::~GL1Renderer() {
    printf("~GLRenderer\n");
}

#endif // __GL1__

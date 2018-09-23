//
// Created by cpasjuste on 06/01/18.
//

#ifndef CROSS2D_RENDERERGL_H
#define CROSS2D_RENDERERGL_H

#include "skeleton/renderer.h"

#define MAX_VERTEX 0x10000

namespace c2d {

    class GLRenderer : public Renderer {

    public:

        GLRenderer(const Vector2f &size = Vector2f(0, 0));

        ~GLRenderer();

        virtual void initGL();

        virtual void draw(VertexArray *vertexArray,
                          const Transform &transform,
                          const Texture *texture);

        virtual void flip(bool draw = true);

        unsigned int vao = 0;
    };

    void CheckOpenGLError(const char *stmt, const char *fname, int line);

#ifndef NDEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

}

#endif //CROSS2D_RENDERERGL_H

//
// Created by cpasjuste on 27/01/17.
//

#ifndef GL_SHADERS_H
#define GL_SHADERS_H

#include "skeleton/shader_list.h"

namespace c2d {

    class GLShader {

    public:

        enum {
            SCALE_TYPE_SOURCE = 0,
            SCALE_TYPE_SCREEN = 1,
            SCALE_TYPE_VIEWPORT = 2
        };

        GLShader(const char *vertex, const char *fragment, int scale_type = SCALE_TYPE_SOURCE);

        ~GLShader();

        void SetUniformMatrix(const GLchar *n, const GLfloat *v);

        void SetUniform(const GLchar *n, const Vector2f &v);

        void SetUniform(const GLchar *n, const float v0, const float v1);

        void SetUniform(const GLchar *n, const int v);

        GLuint GetProgram();

        int getScaleType();

    private:
        GLuint program;
        int scale_type = 0;
        bool available = false;

    };

    class GLShaderList : public ShaderList {

    public:

        GLShaderList(const std::string &shadersPath = "");

        virtual ~GLShaderList();

        Shader *color = nullptr;
    };
}

#endif // GL_SHADERS_H

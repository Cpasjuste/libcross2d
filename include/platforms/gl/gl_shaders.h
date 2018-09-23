//
// Created by cpasjuste on 27/01/17.
//

#ifndef GL_SHADERS_H
#define GL_SHADERS_H

#include "skeleton/shader_list.h"

namespace c2d {

    class GLShader {

    public:
        GLShader(const char *vertex, const char *fragment);

        ~GLShader();

        void SetUniformMatrix(const GLchar *n, const GLfloat *v);

        void SetUniform(const GLchar *n, const Vector2f &v);

        void SetUniform(const GLchar *n, const float v0, const float v1);

        void SetUniform(const GLchar *n, const int v);

        GLuint GetProgram();

    private:
        GLuint program;

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

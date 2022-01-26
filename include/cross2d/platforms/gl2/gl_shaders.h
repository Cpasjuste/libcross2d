//
// Created by cpasjuste on 27/01/17.
//

#ifndef GL_SHADERS_H
#define GL_SHADERS_H

#include "cross2d/skeleton/shader_list.h"

namespace c2d {

    class GLShader {

    public:

        GLShader(const char *source, const std::string &version);

        GLShader(const char *vertex, const char *fragment, int vSize = 0, int fSize = 0);

        ~GLShader();

        void SetUniformMatrix(const GLchar *n, const GLfloat *v);

        void SetUniform(const GLchar *n, const Vector2f &v);

        void SetUniform(const GLchar *n, float v0, float v1);

        void SetUniform(const GLchar *n, int v);

        GLuint GetProgram();

    private:
        GLuint program = -1;
        bool available = false;
    };

    class GLShaderList : public ShaderList {

    public:

        GLShaderList();

        ~GLShaderList() override;

        Shader *color = nullptr;
    };
}

#endif // GL_SHADERS_H

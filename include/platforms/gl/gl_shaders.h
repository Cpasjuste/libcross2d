//
// Created by cpasjuste on 27/01/17.
//

#ifndef GL_SHADERS_H
#define GL_SHADERS_H

#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>

#include "skeleton/shader_list.h"

namespace c2d {

    class GLShader {

    public:
        GLShader(const char *vertex, const char *fragment);

        ~GLShader();

        GLuint program;

        bool available = false;

    };

    class GLShaderList : public ShaderList {

    public:

        GLShaderList(const std::string &shadersPath = "");

        virtual ~GLShaderList();

        Shader *color = nullptr;

        /*
        vita2d_shader *create(const SceGxmProgram *vertexProgramGxp,
                              const SceGxmProgram *fragmentProgramGxp);

        static void setVertexUniform(
                const vita2d_shader *shader,
                const char *param, const float *value, unsigned int length);

        static void setFragmentUniform(
                const vita2d_shader *shader,
                const char *param, const float *value, unsigned int length);

        static void setVertexUniform(
                const SceGxmProgramParameter *program_parameter,
                const float *value, unsigned int length);

        static void setFragmentUniform(
                const SceGxmProgramParameter *program_parameter,
                const float *value, unsigned int length);
        */
    };
}

#endif // GL_SHADERS_H

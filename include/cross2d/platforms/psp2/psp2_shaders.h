//
// Created by cpasjuste on 27/01/17.
//

#ifndef PFBA_PSP2_SHADERS_H
#define PFBA_PSP2_SHADERS_H

#include "cross2d/skeleton/shader_list.h"

typedef struct vita2d_shader_input {
    const SceGxmProgramParameter *video_size;
    const SceGxmProgramParameter *texture_size;
    const SceGxmProgramParameter *output_size;
    const SceGxmProgramParameter *frame_count;
    const SceGxmProgramParameter *frame_direction;
    const SceGxmProgramParameter *frame_rotation;
} vita2d_shader_input;

typedef struct vita2d_shader {
    SceGxmShaderPatcherId vertexProgramId;
    SceGxmShaderPatcherId fragmentProgramId;
    SceGxmVertexProgram *vertexProgram;
    SceGxmFragmentProgram *fragmentProgram;
    const SceGxmProgramParameter *paramPositionAttribute;
    const SceGxmProgramParameter *paramTexcoordAttribute;
    const SceGxmProgramParameter *wvpParam;
    vita2d_shader_input vertexInput;
    vita2d_shader_input fragmentInput;
} vita2d_shader;

namespace c2d {

    class PSP2ShaderList : public ShaderList {

    public:

        PSP2ShaderList(const std::string &shadersPath);

        virtual ~PSP2ShaderList();

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
    };
}

#endif //PFBA_PSP2_SHADERS_H

//
// Created by cpasjuste on 27/01/17.
//

#ifndef PFBA_PSP2_SHADERS_H
#define PFBA_PSP2_SHADERS_H

#include "skeleton/shaders.h"

typedef struct vita2d_shader {
    SceGxmShaderPatcherId vertexProgramId;
    SceGxmShaderPatcherId fragmentProgramId;
    SceGxmVertexProgram *vertexProgram;
    SceGxmFragmentProgram *fragmentProgram;
    const SceGxmProgramParameter *paramPositionAttribute;
    const SceGxmProgramParameter *paramTexcoordAttribute;
    const SceGxmProgramParameter *wvpParam;
} vita2d_shader;

namespace c2d {

    class PSP2Shaders : Shaders {

    public:

        PSP2Shaders(const std::string &shadersPath);

        virtual ~PSP2Shaders();

        vita2d_shader *createShader(const SceGxmProgram *vertexProgramGxp, const SceGxmProgram *fragmentProgramGxp);
    };
}

#endif //PFBA_PSP2_SHADERS_H

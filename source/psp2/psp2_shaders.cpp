//
// Created by cpasjuste on 27/01/17.
//

#include <vita2d.h>
#include "psp2/psp2_shaders.h"

#include "lcd3x_v.h"
#include "lcd3x_f.h"
#include "texture_v.h"
#include "texture_f.h"
#include "advanced_aa_v.h"
#include "advanced_aa_f.h"
#include "scale2x_f.h"
#include "scale2x_v.h"
#include "sharp_bilinear_f.h"
#include "sharp_bilinear_v.h"
#include "sharp_bilinear_simple_f.h"
#include "sharp_bilinear_simple_v.h"
//#include "xbr_2x_noblend_f.h"
//#include "xbr_2x_noblend_v.h"
//#include "fxaa_v.h"
//#include "fxaa_f.h"
//#include "crt_easymode_f.h"
//#include "gtu_v.h"
//#include "gtu_f.h"
//#include "opaque_v.h"
//#include "bicubic_f.h"
//#include "xbr_2x_v.h"
//#include "xbr_2x_f.h"
//#include "xbr_2x_fast_v.h"
//#include "xbr_2x_fast_f.h"

using namespace c2d;

PSP2Shaders::PSP2Shaders(const std::string &shadersPath) : Shaders(shadersPath) {

    // parent class add a "NONE" shader, we set it to a simple texture shader
    get(0)->data = createShader((SceGxmProgram *) texture_v, (SceGxmProgram *) texture_f);

    add("lcd3x",
        createShader((SceGxmProgram *) lcd3x_v, (SceGxmProgram *) lcd3x_f));
    add("sharp",
        createShader((SceGxmProgram *) sharp_bilinear_simple_v, (SceGxmProgram *) sharp_bilinear_simple_f));
    add("sharp+scan",
        createShader((SceGxmProgram *) sharp_bilinear_v, (SceGxmProgram *) sharp_bilinear_f));
    add("aaa",
        createShader((SceGxmProgram *) advanced_aa_v, (SceGxmProgram *) advanced_aa_f));
    add("scale2x",
        createShader((SceGxmProgram *) scale2x_v, (SceGxmProgram *) scale2x_f));

    printf("PSP2Shader: found %i shaders\n", getCount() - 1);
}

PSP2Shaders::~PSP2Shaders() {

    SceGxmShaderPatcher *shaderPatcher = vita2d_get_shader_patcher();

    for (int i = 0; i < getCount(); i++) {
        if (get(i)->data != NULL) {
            vita2d_shader *shader = (vita2d_shader *) get(i)->data;
            sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, shader->fragmentProgram);
            sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, shader->vertexProgram);
            sceGxmShaderPatcherUnregisterProgram(shaderPatcher, shader->fragmentProgramId);
            sceGxmShaderPatcherUnregisterProgram(shaderPatcher, shader->vertexProgramId);
            free(shader);
        }
    }
}

// from frangarcj
vita2d_shader *
PSP2Shaders::createShader(const SceGxmProgram *vertexProgramGxp, const SceGxmProgram *fragmentProgramGxp) {

    vita2d_shader *shader = (vita2d_shader *) malloc(sizeof(*shader));
    SceGxmShaderPatcher *shaderPatcher = vita2d_get_shader_patcher();

    sceGxmProgramCheck(vertexProgramGxp);
    sceGxmProgramCheck(fragmentProgramGxp);
    sceGxmShaderPatcherRegisterProgram(shaderPatcher, vertexProgramGxp, &shader->vertexProgramId);
    sceGxmShaderPatcherRegisterProgram(shaderPatcher, fragmentProgramGxp,
                                       &shader->fragmentProgramId);

    shader->paramPositionAttribute = sceGxmProgramFindParameterByName(vertexProgramGxp, "aPosition");
    shader->paramTexcoordAttribute = sceGxmProgramFindParameterByName(vertexProgramGxp, "aTexcoord");

    // create texture vertex format
    SceGxmVertexAttribute textureVertexAttributes[2];
    SceGxmVertexStream textureVertexStreams[1];
    /* x,y,z: 3 float 32 bits */
    textureVertexAttributes[0].streamIndex = 0;
    textureVertexAttributes[0].offset = 0;
    textureVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
    textureVertexAttributes[0].componentCount = 3; // (x, y, z)
    textureVertexAttributes[0].regIndex = (unsigned short)
            sceGxmProgramParameterGetResourceIndex(shader->paramPositionAttribute);
    /* u,v: 2 floats 32 bits */
    textureVertexAttributes[1].streamIndex = 0;
    textureVertexAttributes[1].offset = 12; // (x, y, z) * 4 = 11 bytes
    textureVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
    textureVertexAttributes[1].componentCount = 2; // (u, v)
    textureVertexAttributes[1].regIndex = (unsigned short)
            sceGxmProgramParameterGetResourceIndex(shader->paramTexcoordAttribute);
    // 16 bit (short) indices
    textureVertexStreams[0].stride = sizeof(vita2d_texture_vertex);
    textureVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

    // create texture shaders
    sceGxmShaderPatcherCreateVertexProgram(
            shaderPatcher,
            shader->vertexProgramId,
            textureVertexAttributes,
            2,
            textureVertexStreams,
            1,
            &shader->vertexProgram);

    // Fill SceGxmBlendInfo
    static const SceGxmBlendInfo blend_info = {
            SCE_GXM_COLOR_MASK_ALL,
            SCE_GXM_BLEND_FUNC_ADD,
            SCE_GXM_BLEND_FUNC_ADD,
            SCE_GXM_BLEND_FACTOR_SRC_ALPHA,
            SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            SCE_GXM_BLEND_FACTOR_ONE,
            SCE_GXM_BLEND_FACTOR_ZERO
    };

    sceGxmShaderPatcherCreateFragmentProgram(
            shaderPatcher,
            shader->fragmentProgramId,
            SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
            SCE_GXM_MULTISAMPLE_NONE,
            &blend_info,
            vertexProgramGxp,
            &shader->fragmentProgram);

    shader->wvpParam = sceGxmProgramFindParameterByName(vertexProgramGxp, "wvp");

    return shader;
}

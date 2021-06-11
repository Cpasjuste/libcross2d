//
// Created by cpasjuste on 27/01/17.
//

#include <vita2d.h>
#include "cross2d/platforms/psp2/psp2_shaders.h"

#include "lcd3x_v.h"
#include "lcd3x_f.h"
#include "texture_v.h"
#include "texture_f.h"
//#include "advanced_aa_v.h"
//#include "advanced_aa_f.h"
//#include "scale2x_f.h"
//#include "scale2x_v.h"
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

PSP2ShaderList::PSP2ShaderList(const std::string &shadersPath) : ShaderList(shadersPath) {
    // parent class add a "NONE" shader, we set it to a simple texture shader
    PSP2ShaderList::get(0)->data = create((SceGxmProgram *) texture_v, (SceGxmProgram *) texture_f);

    PSP2ShaderList::add("lcd3x",
                        create((SceGxmProgram *) lcd3x_v, (SceGxmProgram *) lcd3x_f));
    PSP2ShaderList::add("sharp+scan",
                        create((SceGxmProgram *) sharp_bilinear_v, (SceGxmProgram *) sharp_bilinear_f));
    PSP2ShaderList::add("sharp",
                        create((SceGxmProgram *) sharp_bilinear_simple_v, (SceGxmProgram *) sharp_bilinear_simple_f));
}

// from frangarcj
vita2d_shader *
PSP2ShaderList::create(const SceGxmProgram *vertexProgramGxp, const SceGxmProgram *fragmentProgramGxp) {
    vita2d_shader *shader = (vita2d_shader *) malloc(sizeof(*shader));
    SceGxmShaderPatcher *shaderPatcher = vita2d_get_shader_patcher();

    sceGxmProgramCheck(vertexProgramGxp);
    sceGxmProgramCheck(fragmentProgramGxp);
    sceGxmShaderPatcherRegisterProgram(shaderPatcher, vertexProgramGxp, &shader->vertexProgramId);
    sceGxmShaderPatcherRegisterProgram(shaderPatcher, fragmentProgramGxp,
                                       &shader->fragmentProgramId);

    shader->paramPositionAttribute = sceGxmProgramFindParameterByName(vertexProgramGxp, "aPosition");
    shader->paramTexcoordAttribute = sceGxmProgramFindParameterByName(vertexProgramGxp, "aTexcoord");

    shader->vertexInput.texture_size = sceGxmProgramFindParameterByName(vertexProgramGxp, "IN.texture_size");
    shader->vertexInput.output_size = sceGxmProgramFindParameterByName(vertexProgramGxp, "IN.output_size");
    shader->vertexInput.video_size = sceGxmProgramFindParameterByName(vertexProgramGxp, "IN.video_size");

    shader->fragmentInput.texture_size = sceGxmProgramFindParameterByName(fragmentProgramGxp, "IN.texture_size");
    shader->fragmentInput.output_size = sceGxmProgramFindParameterByName(fragmentProgramGxp, "IN.output_size");
    shader->fragmentInput.video_size = sceGxmProgramFindParameterByName(fragmentProgramGxp, "IN.video_size");

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

void PSP2ShaderList::setVertexUniform(
        const vita2d_shader *shader,
        const char *param, const float *value, unsigned int length) {
    void *vertex_wvp_buffer;
    const SceGxmProgram *program = sceGxmVertexProgramGetProgram(shader->vertexProgram);
    const SceGxmProgramParameter *program_parameter = sceGxmProgramFindParameterByName(program, param);
    sceGxmReserveVertexDefaultUniformBuffer(vita2d_get_context(), &vertex_wvp_buffer);
    sceGxmSetUniformDataF(vertex_wvp_buffer, program_parameter, 0, length, value);
}

void PSP2ShaderList::setVertexUniform(
        const SceGxmProgramParameter *program_parameter, const float *value, unsigned int length) {
    void *vertex_wvp_buffer;
    sceGxmReserveVertexDefaultUniformBuffer(vita2d_get_context(), &vertex_wvp_buffer);
    sceGxmSetUniformDataF(vertex_wvp_buffer, program_parameter, 0, length, value);
}

void PSP2ShaderList::setFragmentUniform(
        const vita2d_shader *shader,
        const char *param, const float *value, unsigned int length) {
    void *fragment_wvp_buffer;
    const SceGxmProgram *program = sceGxmFragmentProgramGetProgram(shader->fragmentProgram);
    const SceGxmProgramParameter *program_parameter = sceGxmProgramFindParameterByName(program, param);
    sceGxmReserveFragmentDefaultUniformBuffer(vita2d_get_context(), &fragment_wvp_buffer);
    sceGxmSetUniformDataF(fragment_wvp_buffer, program_parameter, 0, length, value);
}

void PSP2ShaderList::setFragmentUniform(
        const SceGxmProgramParameter *program_parameter, const float *value, unsigned int length) {
    void *fragment_wvp_buffer;
    sceGxmReserveFragmentDefaultUniformBuffer(vita2d_get_context(), &fragment_wvp_buffer);
    sceGxmSetUniformDataF(fragment_wvp_buffer, program_parameter, 0, length, value);
}

PSP2ShaderList::~PSP2ShaderList() {
    SceGxmShaderPatcher *shaderPatcher = vita2d_get_shader_patcher();
    for (int i = 0; i < PSP2ShaderList::getCount(); i++) {
        if (PSP2ShaderList::get(i)->data != nullptr) {
            auto *shader = (vita2d_shader *) PSP2ShaderList::get(i)->data;
            sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, shader->fragmentProgram);
            sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, shader->vertexProgram);
            sceGxmShaderPatcherUnregisterProgram(shaderPatcher, shader->fragmentProgramId);
            sceGxmShaderPatcherUnregisterProgram(shaderPatcher, shader->vertexProgramId);
            free(shader);
        }
    }
}

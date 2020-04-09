//
// Created by cpasjuste on 21/11/16.
//

#include <citro2d.h>
#include "cross2d/platforms/3ds/ctr_texture.h"
#include "cross2d/platforms/3ds/ctr_renderer.h"

using namespace c2d;

extern "C" {
C2Di_Context __C2Di_Context;
void C2Di_AppendVtx(float x, float y, float z, float u, float v, float ptx, float pty, u32 color);
}

CTRRenderer::CTRRenderer(const Vector2f &size) : Renderer(size) {

    osSetSpeedupEnable(true);

    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(10000);
    C2D_Prepare();

    //consoleInit(GFX_BOTTOM, nullptr);
    consoleDebugInit(debugDevice_SVC);
    stdout = stderr;

    renderTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_FrameEndHook(nullptr, nullptr);

    uLoc_mdlvMtx = shaderInstanceGetUniformLocation(__C2Di_Context.program.vertexShader, "mdlvMtx");
    uLoc_projMtx = shaderInstanceGetUniformLocation(__C2Di_Context.program.vertexShader, "projMtx");
    Mtx_OrthoTilt(&s_projTop, 0.0f, 400.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projMtx, &s_projTop);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_mdlvMtx, &__C2Di_Context.mdlvMtx);

    C3D_TexEnv *env = C3D_GetTexEnv(1);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_RGB, GPU_PREVIOUS, GPU_PRIMARY_COLOR, GPU_TEXTURE3);
    C3D_TexEnvOpRgb(env, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_ONE_MINUS_SRC_ALPHA);
    C3D_TexEnvSrc(env, C3D_Alpha, GPU_PREVIOUS, GPU_PRIMARY_COLOR, (GPU_TEVSRC) 0);
    C3D_TexEnvFunc(env, C3D_RGB, GPU_INTERPOLATE);
    C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);

    available = true;
}

void CTRRenderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture, Sprite *sprite) {

    if (vertexArray == nullptr || vertexArray->getVertexCount() < 1) {
        //printf("gl_render::draw: no vertices\n");
        return;
    }

    Vertex *vertices = vertexArray->getVertices()->data();
    size_t vertexCount = vertexArray->getVertexCount();
    auto *tex = sprite != nullptr ? (CTRTexture *) sprite->getTexture() : (CTRTexture *) texture;
    float blend = 0;

    GPU_Primitive_t type;
    switch (vertexArray->getPrimitiveType()) {
        case PrimitiveType::Triangles:
            type = GPU_TRIANGLES;
            break;
        case PrimitiveType::TriangleStrip:
            type = GPU_TRIANGLE_STRIP;
            break;
        case PrimitiveType::TriangleFan:
            type = GPU_TRIANGLE_FAN;
            break;
        default:
            printf("CTRRenderer::draw: unsupported primitive type\n");
            return;
    }

    if (tex != nullptr) {
        C3D_TexBind(0, &tex->tex);
    }

    C3D_TexEnvSrc(C3D_GetTexEnv(0), C3D_Both,
                  tex != nullptr ? GPU_TEXTURE0 : GPU_CONSTANT, (GPU_TEVSRC) 0, (GPU_TEVSRC) 0);

    for (unsigned int i = 0; i < vertexCount; i++) {

        Vertex v = vertices[i];
        Vector2f pos = transform.transformPoint(v.position);
        blend = v.color.r == 255 && v.color.g == 255 && v.color.b == 255 ? 0 : 1;

        C2Di_AppendVtx(pos.x, pos.y, 0.5f, v.texCoords.x, v.texCoords.y,
                       0, tex != nullptr ? blend : 1.0f, v.color.toABGR());
    }

    size_t len = __C2Di_Context.vtxBufPos - __C2Di_Context.vtxBufLastPos;
    if (len > 0) {
        C3D_DrawArrays(type, __C2Di_Context.vtxBufLastPos, len);
        __C2Di_Context.vtxBufLastPos = __C2Di_Context.vtxBufPos;
    }
}

void CTRRenderer::flip(bool draw, bool inputs) {

    process_inputs = inputs;
    Renderer::onUpdate();

    if (draw) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(renderTarget, m_clearColor.toABGR());
        C2D_SceneBegin(renderTarget);

        Transform trans = Transform::Identity;
        Rectangle::onDraw(trans, draw);

        __C2Di_Context.vtxBufPos = 0;
        __C2Di_Context.vtxBufLastPos = 0;
        C3D_FrameEnd(0);
    }
}

void CTRRenderer::delay(unsigned int ms) {

    s64 nano = ms * 1000000;
    svcSleepThread(nano);
}

CTRRenderer::~CTRRenderer() {

    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

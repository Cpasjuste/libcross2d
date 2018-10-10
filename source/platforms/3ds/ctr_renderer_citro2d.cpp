//
// Created by cpasjuste on 21/11/16.
//

extern "C" {
#include "citro2d.h"
#include "citro2d/source/internal.h"
}

#include "cross2d/platforms/3ds/ctr_texture.h"
#include "cross2d/platforms/3ds/ctr_renderer.h"

using namespace c2d;

CTRRenderer::CTRRenderer(const Vector2f &size) : Renderer(size) {

    osSetSpeedupEnable(true);

    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, nullptr);

    target = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    available = true;
}

void CTRRenderer::draw(VertexArray *vertexArray,
                       const Transform &transform,
                       const Texture *texture) {

    if (!vertexArray || vertexArray->getVertexCount() < 1) {
        printf("ctr_renderer::draw: no vertices\n");
        return;
    }

    C2Di_Context *ctx = C2Di_GetContext();
    if (!(ctx->flags & C2DiF_Active))
        return;
    if (vertexArray->getVertexCount() > (ctx->vtxBufSize - ctx->vtxBufPos))
        return;

    Vertex *vertices = vertexArray->getVertices().data();
    size_t vertexCount = vertexArray->getVertexCount();

    GPU_Primitive_t type;
    switch (vertexArray->getPrimitiveType()) {

        case PrimitiveType::Triangles:
            printf("GPU_TRIANGLES\n");
            type = GPU_TRIANGLES;
            break;

        case PrimitiveType::TriangleStrip:
            printf("GPU_TRIANGLE_STRIP\n");
            type = GPU_TRIANGLE_STRIP;
            break;

        case PrimitiveType::TriangleFan:
            //printf("GPU_TRIANGLE_FAN\n");
            type = GPU_TRIANGLE_FAN;
            break;

        default:
            printf("CTRRenderer::draw: unsupported primitive type\n");
            return;
    }

    if (texture) {
        //C2Di_SetTex(&((CTRTexture *) texture)->tex);
    }
    C2Di_Update();

    for (unsigned int i = 0; i < vertexCount; i++) {
        Vertex v = vertices[i];
        Vector2f pos = transform.transformPoint(v.position);
        unsigned long color = C2D_Color32(
                v.color.r, v.color.g, v.color.b, v.color.a);
        C2Di_AppendVtx(pos.x, pos.y, 0.5f, v.texCoords.x, v.texCoords.y, 1.0f, color);
    }

    /*
    C3D_TexEnv *env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, texture ? GPU_TEXTURE0 : GPU_PRIMARY_COLOR, 0, 0);
    C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);                  // v0=position
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, texture ? 2 : 4);    // v1=texcoord or color

    if (texture) {
        C3D_TexBind(0, &((CTRTexture *) texture)->tex);
    }

    C3D_ImmDrawBegin(type);

    for (unsigned int i = 0; i < vertexCount; i++) {

        Vector2f v = transform.transformPoint(vertices[i].position);

        C3D_ImmSendAttrib(v.x, v.y, 0.5f, 0.0f);
        if (texture) {
            C3D_ImmSendAttrib(
                    vertices[i].texCoords.x / texture->getSize().x,
                    vertices[i].texCoords.y / texture->getSize().y, 0.0f, 0.0f);
        } else {
            float r = (float) vertices[i].color.r / 255.0f;
            float g = (float) vertices[i].color.g / 255.0f;
            float b = (float) vertices[i].color.b / 255.0f;
            float a = (float) vertices[i].color.a / 255.0f;
            C3D_ImmSendAttrib(r, g, b, a);
        }
    }

    C3D_ImmDrawEnd();
    */
}

void CTRRenderer::clear() {

    C2D_TargetClear(target,
                    C2D_Color32(m_clearColor.r,
                                m_clearColor.g,
                                m_clearColor.b,
                                m_clearColor.a));
}

void CTRRenderer::flip(bool draw) {

    if (draw) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        clear();
        C2D_SceneBegin(target);
    }

    // call base class (draw childs)
    Renderer::flip(draw);

    if (draw) {
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

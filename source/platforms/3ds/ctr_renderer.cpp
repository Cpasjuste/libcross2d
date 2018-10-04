//
// Created by cpasjuste on 21/11/16.
//

#include <citro3d.h>
#include "3ds/ctr_renderer.h"

using namespace c2d;

#define CLEAR_COLOR 0x000000FF
#define DISPLAY_TRANSFER_FLAGS \
    (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))
extern const u8 vshader_shbin[];
extern const u32 vshader_shbin_size;

CTRRenderer::CTRRenderer(const Vector2f &size) : Renderer(size) {

    osSetSpeedupEnable(true);

    gfxInitDefault();
    gfxSet3D(false);
    consoleInit(GFX_BOTTOM, NULL);

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    // Initialize the render target
    target = C3D_RenderTargetCreate((int) getSize().x, (int) getSize().y, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    // Load the vertex shader, create a shader program and bind it
    vshader_dvlb = DVLB_ParseFile((u32 *) vshader_shbin, vshader_shbin_size);
    shaderProgramInit(&program);
    shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
    C3D_BindProgram(&program);

    // Get the location of the uniforms
    uloc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

    // Compute the projection matrix
    // Note: we're setting top to height here so origin is at top left.
    Mtx_OrthoTilt(&mtx_projection, 0.0, getSize().x, getSize().y, 0.0, 0.0, 1.0, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uloc_projection, &mtx_projection);

    // Configure depth test to overwrite pixels with the same depth (needed to draw overlapping sprites)
    C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);

    this->shaders = new Shaders("");

    available = true;
}

/*
void CTRRenderer::DrawLine(int x0, int y0, int x1, int y1, const Color &c) {

    float r = (float) c.r / 255.0f;
    float g = (float) c.g / 255.0f;
    float b = (float) c.b / 255.0f;
    float a = (float) c.a / 255.0f;

#ifdef __CITRO3D__
    float dx = x1 - x0;
    float dy = y1 - y0;
    float nx = -dy;
    float ny = dx;
    float len = sqrt(nx * nx + ny * ny);
    if (len > 0) {
        nx /= len;
        ny /= len;
    }
    nx *= 1 * 0.5f; // width
    ny *= 1 * 0.5f; // width

    StartDrawing(true);

    C3D_ImmDrawBegin(GPU_TRIANGLE_STRIP);
    C3D_ImmSendAttrib(x0 + nx, y0 + ny, 0.5f, 0.0f);    // position
    C3D_ImmSendAttrib(r, g, b, a);                      // color
    C3D_ImmSendAttrib(x1 + nx, y1 + ny, 0.5f, 0.0f);
    C3D_ImmSendAttrib(r, g, b, a);
    C3D_ImmSendAttrib(x0 - nx, y0 - ny, 0.5f, 0.0f);
    C3D_ImmSendAttrib(r, g, b, a);
    C3D_ImmSendAttrib(x1 - nx, y1 - ny, 0.5f, 0.0f);
    C3D_ImmSendAttrib(r, g, b, a);
    C3D_ImmDrawEnd();
#else

#endif
}

void CTRRenderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    float r = (float) c.r / 255.0f;
    float g = (float) c.g / 255.0f;
    float b = (float) c.b / 255.0f;
    float a = (float) c.a / 255.0f;

#ifdef __CITRO3D__
    if (fill) {
        StartDrawing(true);

        C3D_ImmDrawBegin(GPU_TRIANGLE_STRIP);
        C3D_ImmSendAttrib(rect.x, rect.y, 0.5f, 0.0f);  // position
        C3D_ImmSendAttrib(r, g, b, a);                  // color
        C3D_ImmSendAttrib(rect.x, rect.y + rect.h, 0.5f, 0.0f);
        C3D_ImmSendAttrib(r, g, b, a);
        C3D_ImmSendAttrib(rect.x + rect.w, rect.y, 0.5f, 0.0f);
        C3D_ImmSendAttrib(r, g, b, a);
        C3D_ImmSendAttrib(rect.x + rect.w, rect.y + rect.h, 0.5f, 0.0f);
        C3D_ImmSendAttrib(r, g, b, a);
        C3D_ImmDrawEnd();
    } else {
        DrawLine(rect.x, rect.y, rect.x + rect.w, rect.y, c);                   // top
        DrawLine(rect.x, rect.y, rect.x, rect.y + rect.h, c);                   // left
        DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, c); // bottom
        DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, c); // right
    }
#else
#endif
}

void CTRRenderer::Clear() {

}
*/

void startDrawing(bool vertexColor) {


    GPU_TEVSRC src = vertexColor ? GPU_PRIMARY_COLOR : GPU_TEXTURE0;

    C3D_TexEnv *env = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env, C3D_Both, src, 0, 0);
    C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);                      // v0=position
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, vertexColor ? 4 : 2);    // v1=texcoord or color
}

void CTRRenderer::draw(const VertexArray &vertices,
                       const Transform &transform,
                       const Texture *texture) {

    unsigned int count = vertices.getVertexCount();

    GPU_Primitive_t type;
    switch (vertices.getPrimitiveType()) {

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

    GPU_TEVSRC src = texture ? GPU_TEXTURE0 : GPU_PRIMARY_COLOR;
    C3D_TexEnv *env = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env, C3D_Both, src, 0, 0);
    C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);                  // v0=position
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, texture ? 2 : 4);    // v1=texcoord or color

    if (texture) {
        //C3D_TexBind(0, &texture->tex);
    }

    C3D_ImmDrawBegin(type);

    for (unsigned int i = 0; i < count; i++) {

        Vector2f v = transform.transformPoint(vertices[i].position);

        C3D_ImmSendAttrib(v.x, v.y, 0.5f, 0.0f);
        if (texture) {
            C3D_ImmSendAttrib(
                    vertices[i].texCoords.x / texture->getSize().x,
                    vertices[i].texCoords.y / texture->getSize().y, 0.0f, 0.0f);
        } else {

        }
    }

    C3D_ImmDrawEnd();
}

void CTRRenderer::flip() {

    C3D_FrameBegin(0);
    C3D_FrameDrawOn(target);

    // call base class (draw childs)
    Renderer::flip();

    C3D_FrameEnd(0);
}

void CTRRenderer::delay(unsigned int ms) {

    s64 nano = ms * 1000000;
    svcSleepThread(nano);
}

CTRRenderer::~CTRRenderer() {

    shaderProgramFree(&program);
    DVLB_Free(vshader_dvlb);
    C3D_RenderTargetDelete(target);
    C3D_Fini();
    gfxExit();
}

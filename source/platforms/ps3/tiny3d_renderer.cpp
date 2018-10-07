//
// Created by cpasjuste on 21/11/16.
//

#include "cross2d/c2d.h"
#include <cmath>
#include <sysutil/sysutil.h>
#include <sysmodule/sysmodule.h>

using namespace c2d;

static void quit() {

    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    sysModuleUnload(SYSMODULE_PNGDEC);
}

static void eventHandle(u64 status, u64 param, void *userdata) {
    (void) param;
    (void) userdata;
    if (status == SYSUTIL_EXIT_GAME
        || status == SYSUTIL_MENU_OPEN) {
        exit(0);
    }
}

TINY3DRenderer::TINY3DRenderer(const Vector2f &size) : Renderer(size) {

    atexit(quit);

    sysModuleLoad(SYSMODULE_PNGDEC);
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, eventHandle, NULL);

    tiny3d_Init(1024 * 1024);

    setSize(Vector2f(850, 514));
    setTextureRect(IntRect(0, 0, 850, 514));

    this->shaders = new Shaders("");
}

void TINY3DRenderer::setShader(int shader) {

}

void TINY3DRenderer::draw(const VertexArray &vertices,
                          const Transform &transform,
                          const Texture *texture) {

    unsigned int count = vertices.getVertexCount();

    switch (vertices.getPrimitiveType()) {

        case PrimitiveType::Points:
            tiny3d_SetPolygon(TINY3D_POINTS);
            break;

        case PrimitiveType::Lines:
            tiny3d_SetPolygon(TINY3D_LINES);
            break;

        case PrimitiveType::LineStrip:
            tiny3d_SetPolygon(TINY3D_LINE_STRIP);
            break;

        case PrimitiveType::Triangles:
            tiny3d_SetPolygon(TINY3D_TRIANGLES);
            break;


        case PrimitiveType::TriangleStrip:
            tiny3d_SetPolygon(TINY3D_TRIANGLE_STRIP);
            break;

        case PrimitiveType::TriangleFan:
            tiny3d_SetPolygon(TINY3D_TRIANGLE_FAN);
            break;

        default:
            printf("PSP2Render::draw: unsupported primitive type\n");
            return;
    }

    TINY3DTexture *tex = ((TINY3DTexture *) texture);
    if (tex) {
        tiny3d_SetTexture(0, tex->offset,
                          (u32) tex->getSize().x, (u32) tex->getSize().y,
                          (u32) tex->pitch, tex->fmt, TEXTURE_LINEAR);
    }

    for (unsigned int i = 0; i < count; i++) {

        Vector2f v = transform.transformPoint(vertices[i].position);
        tiny3d_VertexPos(v.x, v.y, 0);
        tiny3d_VertexColor(vertices[i].color.toRGBA());
        if (tex) {
            tiny3d_VertexTexture(
                    vertices[i].texCoords.x / texture->getSize().x,
                    vertices[i].texCoords.y / texture->getSize().y);
        }
    }

    tiny3d_End();
}

void TINY3DRenderer::flip() {

    // clear
    tiny3d_Clear(getFillColor().toARGB(), TINY3D_CLEAR_ALL);
    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);
    // Enable alpha blending.
    tiny3d_BlendFunc(1,
                     (blend_src_func)(TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA),
                     (blend_dst_func)(NV30_3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA |
                                      NV30_3D_BLEND_FUNC_DST_ALPHA_ZERO),
                     (blend_func)(TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD));
    tiny3d_Project2D();

    // call base class (draw childs)
    Renderer::flip();

    // flip
    tiny3d_Flip();

    sysUtilCheckCallback();
}

void TINY3DRenderer::delay(unsigned int ms) {
    usleep(ms * 1000);
}

TINY3DRenderer::~TINY3DRenderer() {

    sysModuleUnload(SYSMODULE_PNGDEC);
}

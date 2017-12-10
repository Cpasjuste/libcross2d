//
// Created by cpasjuste on 21/11/16.
//

#include <C2D.h>
#include <unistd.h>
#include <sysmodule/sysmodule.h>
#include "tiny3d.h"

//////////
// INIT //
//////////
TINY3DRenderer::TINY3DRenderer(int w, int h) : Renderer(w, h) {

    sysModuleLoad(SYSMODULE_PNGDEC);

    tiny3d_Init(1024 * 1024);

    width = 850;
    height = 514;

    this->shaders = new Shaders("");
}
//////////
// INIT //
//////////

void TINY3DRenderer::SetShader(int shader) {

}

void TINY3DRenderer::DrawLine(int x0, int y0, int x1, int y1, const Color &c) {

    float dx = x1 - x0;
    float dy = y1 - y0;
    float nx = -dy;
    float ny = dx;
    float len = sqrt(nx * nx + ny * ny);

    if (len > 0) {
        nx /= len;
        ny /= len;
    }

    float line_w = 1;
    float line_h = 1;
    nx *= line_w * 0.5f;
    ny *= line_h * 0.5f;

    Color col = c;

    tiny3d_SetPolygon(TINY3D_TRIANGLE_STRIP);
    tiny3d_VertexPos(x0 + nx, y0 + ny, 0);
    tiny3d_VertexColor(col.ToRGBA());
    tiny3d_VertexPos(x0 - nx, y0 - ny, 0);
    tiny3d_VertexPos(x1 + nx, y1 + ny, 0);
    tiny3d_VertexPos(x1 - nx, y1 - ny, 0);

    tiny3d_End();
}

void TINY3DRenderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    if (fill) {

        Color col = c;

        tiny3d_SetPolygon(TINY3D_TRIANGLE_STRIP);
        tiny3d_VertexPos((float) rect.x, (float) rect.y, 0);
        tiny3d_VertexColor(col.ToRGBA());
        tiny3d_VertexPos((float) rect.x + rect.w, (float) rect.y, 0);
        tiny3d_VertexPos((float) rect.x, (float) rect.y + rect.h, 0);
        tiny3d_VertexPos((float) rect.x + rect.w, (float) rect.y + rect.h, 0);
        tiny3d_End();
    } else {
        DrawLine(rect.x, rect.y + 1, rect.x + rect.w, rect.y + 1, c);
        DrawLine(rect.x + 1, rect.y, rect.x + 1, rect.y + rect.h, c);
        DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, c);
        DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, c);
    }
}

void TINY3DRenderer::Clear() {

    tiny3d_Clear(color.ToARGB(), TINY3D_CLEAR_ALL);
    clear = true;
}

void TINY3DRenderer::Flip() {

    tiny3d_Flip();

    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

    // Enable alpha blending.
    tiny3d_BlendFunc(1,
                     (blend_src_func) (TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA),
                     (blend_dst_func) (NV30_3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA |
                                       NV30_3D_BLEND_FUNC_DST_ALPHA_ZERO),
                     (blend_func) (TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD));

    tiny3d_Project2D();

    // needs to be called
    if (!clear) {
        tiny3d_Clear(color.ToARGB(), TINY3D_CLEAR_ALL);
    } else {
        clear = false;
    }
}

void TINY3DRenderer::Delay(unsigned int ms) {
    usleep(ms * 1000);
}

TINY3DRenderer::~TINY3DRenderer() {

    sysModuleUnload(SYSMODULE_PNGDEC);
}

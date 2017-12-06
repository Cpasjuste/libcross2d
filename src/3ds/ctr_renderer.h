//
// Created by cpasjuste on 21/11/16.
//

#ifndef _CTR_RENDERER_H_
#define _CTR_RENDERER_H_

#include "renderer.h"

class CTRRenderer : Renderer {

public:
    CTRRenderer(int w, int h);

    ~CTRRenderer();

    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    void DrawRect(const Rect &rect, const Color &color, bool fill = true);

    void Clear();

    void Flip();

    void Delay(unsigned int ms);

    void StartDrawing(bool vertexColor);

    C3D_RenderTarget *target;

private:

    bool drawing_started = false;
    int uloc_projection;
    C3D_Mtx mtx_projection;
    DVLB_s *vshader_dvlb;
    shaderProgram_s program;
};

#endif //_CTR_RENDERER_H_

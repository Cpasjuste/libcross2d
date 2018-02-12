//
// Created by cpasjuste on 21/11/16.
//

#ifndef _CTR_RENDERER_H_
#define _CTR_RENDERER_H_

#include "skeleton/renderer.h"

namespace c2d {

    class CTRRenderer : public Renderer {

    public:

        CTRRenderer(const Vector2f &size = Vector2f(0, 0));

        ~CTRRenderer();

        void draw(const VertexArray &vertices,
                  const Transform &transform,
                  const Texture *texture);

        void flip();

        void delay(unsigned int ms);

        C3D_RenderTarget *target;

    private:

        int uloc_projection;
        C3D_Mtx mtx_projection;
        DVLB_s *vshader_dvlb;
        shaderProgram_s program;
    };
}

#endif //_CTR_RENDERER_H_

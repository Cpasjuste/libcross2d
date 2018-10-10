//
// Created by cpasjuste on 21/11/16.
//

#ifndef C2D_CTR_RENDERER_H_
#define C2D_CTR_RENDERER_H_

#include "cross2d/skeleton/renderer.h"
#include "citro3d.h"

namespace c2d {

    class CTRRenderer : public Renderer {

    public:

        CTRRenderer(const Vector2f &size = Vector2f(0, 0));

        ~CTRRenderer();

        void draw(VertexArray *vertexArray,
                  const Transform &transform,
                  const Texture *texture) override;

        void clear() override;

        void flip(bool draw = true) override;

        void delay(unsigned int ms) override;

        C3D_RenderTarget *target;

    private:

        int uloc_projection;
        C3D_Mtx mtx_projection;
        DVLB_s *vshader_dvlb;
        shaderProgram_s program;
    };
}

#endif //C2D_CTR_RENDERER_H_

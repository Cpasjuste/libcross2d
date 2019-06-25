//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL1_RENDERER_H
#define SDL1_RENDERER_H

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class SDL1Renderer : public Renderer {

    public:

        explicit SDL1Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL1Renderer() override;

        void draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) override;

        void clear() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

    private:

        SDL_Surface *screen = nullptr;
    };
}

#endif //SDL1_RENDERER_H

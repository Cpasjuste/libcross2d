//
// Created by cpasjuste on 21/11/16.
//

#ifndef DREAMCAST_RENDERER_H
#define DREAMCAST_RENDERER_H

#include "cross2d/platforms/dreamcast/dreamcast_renderer.h"

namespace c2d {

    class DCRenderer : public Renderer {

    public:

        explicit DCRenderer(const Vector2f &size = Vector2f(0, 0));

        ~DCRenderer() override;

        virtual void flip(bool draw = true, bool inputs = true);

        virtual void delay(unsigned int ms);

    private:

    };
}

#endif // DREAMCAST_RENDERER_H

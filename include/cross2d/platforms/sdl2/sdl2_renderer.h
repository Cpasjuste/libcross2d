//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>

namespace c2d {

    class SDL2Renderer : public Renderer {

    public:

        explicit SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer() override;

        void draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) override;

        void clear() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

        SDL_Renderer *renderer = nullptr;

    private:

        SDL_Window *window = nullptr;
    };
}

#endif // SDL2_RENDERER_H

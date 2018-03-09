//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

namespace c2d {

    class SDL2Renderer : public Renderer {

    public:

        SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer();

        void draw(const VertexArray &vertices,
                  const Transform &transform,
                  const Texture *texture);

        void flip(bool draw = true);

        void delay(unsigned int ms);

        SDL_Renderer *renderer = NULL;
        SDL_Texture *textureShape = NULL;

    private:

        SDL_Window *window = NULL;
    };
}

#endif // SDL2_RENDERER_H

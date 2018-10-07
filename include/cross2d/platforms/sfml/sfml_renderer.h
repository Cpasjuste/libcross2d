//
// Created by cpasjuste on 21/11/16.
//

#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include "cross2d/skeleton/renderer.h"
#include "sfml_shaders.h"

namespace c2d {

    class SFMLRenderer : public Renderer {

    public:

        SFMLRenderer(const Vector2f &size = Vector2f(0, 0), const std::string &shaderPath = "");

        ~SFMLRenderer();

        void draw(const VertexArray &vertices,
                  const Transform &transform,
                  const Texture *texture);

        void flip();

        void delay(unsigned int ms);

        void setShader(int index);

        //private:
        sf::RenderWindow window;
        sf::VideoMode mode;
    };
}

#endif //SFML_RENDERER_H

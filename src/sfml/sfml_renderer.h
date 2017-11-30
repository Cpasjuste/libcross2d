//
// Created by cpasjuste on 21/11/16.
//

#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include "skeleton/renderer.h"
#include "sfml_shaders.h"
#include <SFML/Graphics.hpp>

class SFMLRenderer : Renderer {

public:
    SFMLRenderer(int w, int h, const std::string &shaderPath = "");

    virtual ~SFMLRenderer();

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    virtual void DrawRect(const Rect &rect, const Color &color, bool fill);

    virtual void Clip(const Rect &rect);

    virtual void Clear();

    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int index);

    sf::RenderWindow window;
    sf::VideoMode mode;
};

#endif //SFML_RENDERER_H

//
// Created by cpasjuste on 21/11/16.
//

#include "GL/gl.h"
#include "sfml_renderer.h"
#include "sfml_font.h"
#include "sfml_texture.h"

//////////
// INIT //
//////////
SFMLRenderer::SFMLRenderer(int w, int h, const std::string &shaderPath) : Renderer(w, h) {

    sf::ContextSettings settings(16, 0, 0);
    mode = sf::VideoMode::getDesktopMode();
    sf::Uint32 style = sf::Style::Fullscreen;

    // windowed
    if (w != 0 && h != 0) {
        mode.width = (unsigned int) w;
        mode.height = (unsigned int) h;
        style = sf::Style::Default;
    }

    window.create(mode, "SFMLRenderer", style, settings);
    window.setVerticalSyncEnabled(true);

    printf("SFMLRenderer: %i x %i @ %i bpp\n", mode.width, mode.height, mode.bitsPerPixel);
    const unsigned char *glversion = glGetString(GL_VERSION);
    printf("SFMLRenderer: glversion: %s\n", glversion);
    const unsigned char *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("SFMLRenderer: glslversion: %s\n", glslversion);

    this->shaders = (Shaders *) new SFMLShaders(shaderPath);
}
//////////
// INIT //
//////////

void SFMLRenderer::SetShader(int index) {
    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;
}

void SFMLRenderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    sf::Color col(c.r, c.g, c.b, c.a);

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), col),
            sf::Vertex(sf::Vector2f(x2, y2), col)
    };

    window.draw(line, 2, sf::Lines);
}

void SFMLRenderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    sf::Color col(c.r, c.g, c.b, c.a);
    sf::RectangleShape rectangle(sf::Vector2f(rect.w - 2, rect.h - 2));
    rectangle.setOutlineColor(col);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(rect.x + 1, rect.y + 1);
    if (fill) {
        rectangle.setFillColor(col);
    } else {
        rectangle.setFillColor(sf::Color(0, 0, 0, 0));
    }
    window.draw(rectangle);
}

void SFMLRenderer::Clip(const Rect &rect) {
}

void SFMLRenderer::Clear() {
    window.clear(
            sf::Color(color.r, color.g, color.b, color.a));
}

void SFMLRenderer::Flip() {
    window.display();
}

void SFMLRenderer::Delay(unsigned int ms) {
    sf::sleep(sf::milliseconds(ms));
}

SFMLRenderer::~SFMLRenderer() {
    delete (shaders);
    window.close();
}

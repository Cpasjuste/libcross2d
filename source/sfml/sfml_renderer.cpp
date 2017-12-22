//
// Created by cpasjuste on 21/11/16.
//

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

//////////
// INIT //
//////////
SFMLRenderer::SFMLRenderer(const Vector2f &size, const std::string &shaderPath) : Renderer(size) {

    sf::ContextSettings settings(16, 0, 0);
    mode = sf::VideoMode::getDesktopMode();
    sf::Uint32 style = sf::Style::Fullscreen;

    // windowed
    if (getSize().x != 0 && getSize().y != 0) {
        mode.width = (unsigned int) getSize().x;
        mode.height = (unsigned int) getSize().y;
        style = sf::Style::Default;
    }

    window.create(mode, "SFMLRenderer", style, settings);
    window.setVerticalSyncEnabled(true);

    printf("SFMLRenderer: %i x %i @ %i bpp\n", mode.width, mode.height, mode.bitsPerPixel);
    const unsigned char *glversion = glGetString(GL_VERSION);
    printf("SFMLRenderer: glversion: %s\n", glversion);
    const unsigned char *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("SFMLRenderer: glslversion: %s\n", glslversion);

    if (shaders) {
        delete (shaders);
    }
    shaders = (Shaders *) new SFMLShaders(shaderPath);
}
//////////
// INIT //
//////////

void SFMLRenderer::setShader(int index) {

    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;
}

/*
void SFMLRenderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    sf::Color col(c.r, c.g, c.b, c.a);

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), col),
            sf::Vertex(sf::Vector2f(x2, y2), col)
    };

    window.draw(line, 2, sf::Lines);
}
*/

void SFMLRenderer::drawRectangle(Rectangle &rectangle, Transform &transform) {

    sf::RectangleShape rect((const sf::Vector2f &) rectangle.getSize());
    rect.setPosition((const sf::Vector2f &) rectangle.getPosition());
    rect.setOrigin((const sf::Vector2f &) rectangle.getOrigin());
    rect.setScale((const sf::Vector2f &) rectangle.getScale());
    rect.setRotation(rectangle.getRotation());

    rect.setFillColor((const sf::Color &) rectangle.getFillColor());
    rect.setOutlineColor((const sf::Color &) rectangle.getOutlineColor());
    rect.setOutlineThickness(rectangle.getOutlineThickness());

    sf::RenderStates states;
    states.transform = ((sf::Transform &) transform);

    window.draw(rect, states);
}

void SFMLRenderer::drawTexture(Texture &texture, Transform &transform) {

    sf::Sprite sprite = ((SFMLTexture *) &texture)->sprite;
    sprite.setPosition((const sf::Vector2f &) texture.getPosition());
    sprite.setOrigin((const sf::Vector2f &) texture.getOrigin());
    sprite.setScale((const sf::Vector2f &) texture.getScale());
    sprite.setRotation(texture.getRotation());

    sprite.setColor((const sf::Color &) texture.getFillColor());

    sf::RenderStates states;
    states.transform = ((sf::Transform &) transform);

    // set sprite shader
    sf::Shader *shader = (sf::Shader *) shaders->Get()->data;
    if (shader) {
        shader->setUniform("Texture", (const sf::Texture &) texture);
        shader->setUniform("MVPMatrix", sf::Glsl::Mat4(
                window.getView().getTransform().getMatrix()));
        shader->setUniform("TextureSize", sf::Glsl::Vec2(
                texture.getTextureRect().width,
                texture.getTextureRect().height));
        shader->setUniform("InputSize", sf::Glsl::Vec2(
                texture.getTextureRect().width,
                texture.getTextureRect().height));
        shader->setUniform("OutputSize", sf::Glsl::Vec2(
                texture.getSize().x,
                texture.getSize().y));
        states.shader = shader;
    }

    window.draw(sprite, states);
}

void SFMLRenderer::flip() {

    // clear screen
    window.clear((const sf::Color &) getFillColor());

    // call base class (draw childs)
    Renderer::flip();

    // flip
    window.display();
}

void SFMLRenderer::delay(unsigned int ms) {

    sf::sleep(sf::milliseconds(ms));
}

SFMLRenderer::~SFMLRenderer() {

    delete (shaders);
    window.close();
}

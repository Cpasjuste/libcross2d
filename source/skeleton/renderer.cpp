//
// Created by cpasjuste on 09/12/16.
//

#include "c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : RectangleShape(size) {

    thisTransform = this;
    c2d_renderer = this;

    setFillColor(Color::Black);

    printf("Renderer(%p)\n", this);
}

void Renderer::drawLine(Line &line, Transform &transform) {

    Transform combined = transform * line.getTransform();
    draw(line.getVertices(), combined, nullptr);
    if (line.getOutlineThickness() > 0) {
        draw(line.getOutlineVertices(), combined, nullptr);
    }
}

void Renderer::drawRectangle(Rectangle &rectangle, Transform &transform) {

    Transform combined = transform * rectangle.getTransform();
    draw(rectangle.getVertices(), combined, nullptr);
    if (rectangle.getOutlineThickness() > 0) {
        draw(rectangle.getOutlineVertices(), combined, nullptr);
    }
}

void Renderer::drawTexture(Texture &texture, Transform &transform) {

    Transform combined = transform * texture.getTransform();
    draw(texture.getVertices(), combined, &texture);
}

void Renderer::drawText(Text &text, Transform &transform) {

    Transform combined = transform * text.getTransform();
    if (text.getOutlineThickness() > 0) {
        draw(text.getOutlineVertices(), combined, &text.getFont()->getTexture(text.getCharacterSize()));
    }
    draw(text.getVertices(), combined, &text.getFont()->getTexture(text.getCharacterSize()));
}

void Renderer::flip() {

    printf("Renderer(%p): flip\n", this);

    // call base class (draw childs)
    Transform trans = getTransform();
    Widget::draw(trans);
}

void Renderer::setShader(int shader) {

}

void Renderer::delay(unsigned int ms) {

}

Renderer::~Renderer() {

    printf("~Renderer(%p)\n", this);
}

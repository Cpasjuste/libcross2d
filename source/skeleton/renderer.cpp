//
// Created by cpasjuste on 09/12/16.
//

#include "c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : RectangleShape(size) {

    c2d_renderer = this;
    thisTransform = this;
    setFillColor(Color::Black);

    deltaClock = new C2DClock();
    elapsedClock = new C2DClock();
    //printf("Renderer(%p)\n", this);
}

void Renderer::drawLine(Line &line, Transform &transform) {

    Transform combined = transform * line.getTransform();
    if (line.getFillColor().a != 0) {
        draw(line.getVertices(), combined, nullptr);
    }
    if (line.getOutlineColor().a != 0 && line.getOutlineThickness() > 0) {
        draw(line.getOutlineVertices(), combined, nullptr);
    }
}

void Renderer::drawRectangle(Rectangle &rectangle, Transform &transform) {

    //printf("drawRectangle\n");
    Transform combined = transform * rectangle.getTransform();
    if (rectangle.getFillColor().a != 0) {
        draw(rectangle.getVertices(), combined, nullptr);
    }
    if (rectangle.getOutlineColor().a != 0 && rectangle.getOutlineThickness() > 0) {
        draw(rectangle.getOutlineVertices(), combined, nullptr);
    }
}

void Renderer::drawCircle(Circle &circle, Transform &transform) {

    //printf("drawRectangle\n");
    Transform combined = transform * circle.getTransform();
    if (circle.getFillColor().a != 0) {
        draw(circle.getVertices(), combined, nullptr);
    }
    if (circle.getOutlineColor().a != 0 && circle.getOutlineThickness() > 0) {
        draw(circle.getOutlineVertices(), combined, nullptr);
    }
}

void Renderer::drawTexture(Texture &texture, Transform &transform) {

    //printf("drawTexture: %ix%i (%s)\n", (int)texture.getSize().x, (int)texture.getSize().y, texture.path);
    Transform combined = transform * texture.getTransform();

    // TODO
    //if (texture.getOutlineColor().a != 0 && texture.getOutlineThickness() > 0) {
    //    draw(texture.getOutlineVertices(), combined, nullptr);
    //}
    if (texture.available && texture.getColor().a != 0) {
        draw(texture.getVertices(), combined, &texture);
    }
}

void Renderer::drawText(Text &text, Transform &transform) {

    //printf("drawText: %s\n", text.getString().toAnsiString().c_str());
    Transform combined = transform * text.getTransform();

    // fix top not at 0 if needed (font->setYOffset)
    float scale = text.getCharacterSize() / (float) C2D_DEFAULT_CHAR_SIZE;
    combined.translate(0, (text.getFont()->getYOffset() * scale) + text.getOutlineThickness());

    //
    if (text.getOutlineThickness() > 0) {
        draw(text.getOutlineVertices(), combined, &text.getFont()->getTexture(text.getCharacterSize()));
    }
    draw(text.getVertices(), combined, &text.getFont()->getTexture(text.getCharacterSize()));
}

void Renderer::flip(bool draw) {

    //printf("Renderer(%p): flip\n", this);

    // call base class (draw childs)
    if (draw) {
        Transform trans = getTransform();
        C2DObject::draw(trans);
    }

    deltaTime = deltaClock->restart();
    elapsedTime = elapsedClock->getElapsedTime();

    frames++;
    time_now = elapsedTime.asSeconds();
    if (time_now - time_last >= 1.0f) {
        fps = frames;
        frames = 0;
        time_last = time_now;
    }
}

Time Renderer::getDeltaTime() const {

    return deltaTime;
}

Time Renderer::getElapsedTime() const {

    return elapsedTime;
}

float Renderer::getFps() const {

    return fps;
}

void Renderer::setShaderList(ShaderList *list) {
    shaderList = list;
}

ShaderList *Renderer::getShaderList() {
    return shaderList;
}

Renderer::~Renderer() {

    printf("~Renderer(%p)\n", this);

    delete (deltaClock);
    delete (elapsedClock);
    if (shaderList) {
        delete (shaderList);
    }
}

//
// Created by cpasjuste on 09/12/16.
//

#include "C2D.h"

using namespace C2D;

Renderer *C2DMainRenderer;

Renderer::Renderer(int x, int y, int w, int h)
        : Widget(x, y, w, h) {

    shaders = new Shaders("");

    printf("Renderer(%p): x:%i, y:%i, w:%i, h:%i\n", this,
           (int) rect_origin.x, (int) rect_origin.y,
           (int) rect_origin.w, (int) rect_origin.h);
}

void Renderer::DrawLine(Line *line) {

    printf("Renderer(%p): DrawLine\n", this);
}

void Renderer::DrawRectangle(Rectangle *rectangle) {

    printf("Renderer(%p): DrawRectangle\n", this);
}

/*
void Renderer::Clear() {

    ((Renderer *) renderer)->Clear();
}
*/

void Renderer::Flip() {

    printf("Renderer(%p): Flip\n", this);

    // clear
    //((Renderer *) renderer)->Clear();
    // update (this) widget position/scaling
    //Widget::Update();

    // call base class (draw childs)
    Widget::draw(this);
}

void Renderer::SetShader(int shader) {

}

void Renderer::Delay(unsigned int ms) {

}

Renderer::~Renderer() {
    printf("~Renderer(%p)\n", this);
}

//
// Created by cpasjuste on 09/12/16.
//

#include "C2D.h"

using namespace C2D;

Renderer *C2DMainRenderer;

Renderer::Renderer(int x, int y, int w, int h)
        : Widget(x, y, w, h) {

    C2DMainRenderer = this;
    shaders = new Shaders("");

    printf("Renderer(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void Renderer::Draw() {
    printf("Renderer(%p): Draw\n", this);
    Widget::Draw();
}

Renderer::~Renderer() {
    printf("~Renderer(%p)\n", this);
}

/*
void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    DrawLine(x1, y1, x2, y2, color);
}

void Renderer::DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool fill) {
    Rect rect{x, y, w, h};
    Color color{r, g, b, a};
    DrawRect(rect, color, fill);
}
*/

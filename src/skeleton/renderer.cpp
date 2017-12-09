//
// Created by cpasjuste on 09/12/16.
//

#include "renderer.h"

Renderer::Renderer(int w, int h) {
    width = w;
    height = h;
    color = C2D_COL_BLACK;
}

Renderer::~Renderer() {
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    DrawLine(x1, y1, x2, y2, color);
}

void Renderer::DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool fill) {
    Rect rect{x, y, w, h};
    Color color{r, g, b, a};
    DrawRect(rect, color, fill);
}


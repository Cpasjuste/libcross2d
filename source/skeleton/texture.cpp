//
// Created by cpasjuste on 29/11/17.
//

#include "../../include/skeleton/renderer.h"

using namespace C2D;

Texture::Texture(const char *p) : Widget( 0, 0, 0, 0) {

    strncpy(path, p, 512);
}

Texture::Texture(Renderer *r, int w, int h) {

    renderer = r;
    width = w;
    height = h;
}

void Texture::Draw(int x, int y, int w, int h) {
    Draw(x, y, w, h, 0);
}

void Texture::Draw(int x, int y) {
    Draw(x, y, width, height);
}

Rect Texture::Draw(const Rect &rect, bool fit) {

    Rect scale;
    if (fit) {
        scale = {rect.x, rect.y, rect.w, rect.h};
        scale.w = (int) (((float) width * ((float) rect.h) / (float) height));
    } else {
        scale = {rect.x, rect.y, width, height};
    }

    if (scale.w > rect.w) {
        scale.h = (int) (((float) rect.w * ((float) height) / (float) width));
        scale.w = rect.w;
    }

    scale.x += (rect.w - scale.w) / 2;
    scale.y += (rect.h - scale.h) / 2;

    Draw(scale.x, scale.y, scale.w, scale.h, 0);

    return scale;
}


//
// Created by cpasjuste on 29/11/17.
//

#include "c2d.h"

using namespace c2d;

Texture::Texture(const char *p) : RectangleShape(Vector2f(0, 0)) {

    printf("Texture(%p): %s\n", this, p);

    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;

    setFillColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    strncpy(path, p, 512);

    thisTransform = this;
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : RectangleShape(Vector2f(0, 0)) {

    printf("Texture(%p)\n", this);

    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;

    setFillColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    thisTransform = this;
}

Texture::Texture(const Vector2f &size, int fmt) : RectangleShape(size) {

    printf("Texture(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);

    format = fmt;
    bpp = fmt == C2D_TEXTURE_FMT_RGBA8 ? 4 : 2;
    pitch = (int) (size.x * bpp);

    setSize(size);
    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFillColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    thisTransform = this;
}

void Texture::draw(Transform &transform) {

    // draw texture from renderer
    c2d_renderer->drawTexture(*this, transform);

    // call base class (draw childs)
    C2DObject::draw(transform);
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}

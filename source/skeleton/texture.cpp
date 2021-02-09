//
// Created by cpasjuste on 29/11/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

Texture::Texture(const std::string &p) : RectangleShape(Vector2f{0, 0}) {

    printf("Texture(%p): %s\n", this, p.c_str());

    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;
    path = p;

    setFillColor(Color::White);
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : RectangleShape(Vector2f{0, 0}) {

    printf("Texture(%p)\n", this);

    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;

    setFillColor(Color::White);
}

Texture::Texture(const Vector2f &size, Format fmt) : RectangleShape(Vector2f{0, 0}) {

    printf("Texture(%p): %i x %i\n", this, (int) size.x, (int) size.y);

    type = Type::Texture;
    format = fmt;
    bpp = fmt == Format::RGB565 ? 2 : 4;
    pitch = (int) ((int) size.x * bpp);

    setFillColor(Color::White);
    setSize(size);
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}

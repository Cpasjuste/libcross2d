//
// Created by cpasjuste on 29/11/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

Texture::Texture(const std::string &p) : Shape() {

    printf("Texture(%p): %s\n", this, p.c_str());

    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;
    path = p;

    setFillColor(Color::White);
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : Shape() {

    printf("Texture(%p)\n", this);

    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;

    setFillColor(Color::White);
}

Texture::Texture(const Vector2f &size, Format fmt) : Shape() {

    printf("Texture(%p): %i x %i\n", this, (int) size.x, (int) size.y);

    type = Type::Texture;
    format = fmt;
    bpp = fmt == Format::RGB565 ? 2 : 4;
    pitch = (int) (size.x * bpp);

    setFillColor(Color::White);
    setSize(size);
}

void Texture::setSize(const Vector2f &size) {
    m_size = size;
    update();
}

void Texture::setSize(float width, float height) {
    m_size.x = width;
    m_size.y = height;
    update();
}

const Vector2f &Texture::getSize() const {
    return m_size;
}

std::size_t Texture::getPointCount() const {
    return 4;
}

Vector2f Texture::getPoint(std::size_t index) const {
    switch (index) {
        default:
        case 0:
            return {0, 0};
        case 1:
            return {m_size.x, 0};
        case 2:
            return {m_size.x, m_size.y};
        case 3:
            return {0, m_size.y};
    }
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}

//
// Created by cpasjuste on 29/11/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

Texture::Texture(const std::string &p) : RectangleShape(Vector2f{0, 0}) {
    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;
    path = p;
    Texture::setFillColor(Color::White);
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : RectangleShape(Vector2f{0, 0}) {
    type = Type::Texture;
    format = Format::RGBA8;
    bpp = 4;
    Texture::setFillColor(Color::White);
}

Texture::Texture(const Vector2f &size, Format fmt) : RectangleShape(Vector2f{0, 0}) {
    type = Type::Texture;
    format = fmt;
    bpp = fmt == Format::RGB565 ? 2 : 4;
    pitch = (int) ((int) size.x * bpp);
    Texture::setFillColor(Color::White);
    Texture::setSize(size);
}

void Texture::setShader(int shaderIndex) {
    ShaderList *shaderList = c2d_renderer->getShaderList();
    shader = shaderList->get(shaderIndex);
    if (!shader) {
        printf("Texture::setShader: shader not found (%i)\n", shaderIndex);
        shader = shaderList->get(0);
    }
}

void Texture::setShader(const std::string &shaderName) {
    ShaderList *shaderList = c2d_renderer->getShaderList();
    shader = shaderList->get(shaderName);
    if (!shader) {
        printf("Texture::setShader: shader not found (%s)\n", shaderName.c_str());
        shader = shaderList->get(0);
    }
}

Texture::~Texture() {
}

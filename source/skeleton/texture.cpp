//
// Created by cpasjuste on 29/11/17.
//

#include <c2d.h>

using namespace c2d;

Texture::Texture(const char *p) : Widget(Vector2f(0, 0)) {

    printf("Texture(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);

    strncpy(path, p, 512);
}

Texture::Texture(const Vector2f &size) : Widget(size) {

    printf("Texture(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);

    this->setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
}

void Texture::draw(Transform &transform) {

    // draw texture from renderer
    c2d_renderer->drawTexture(*this, transform);

    // call base class (draw childs)
    Widget::draw(transform);
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}

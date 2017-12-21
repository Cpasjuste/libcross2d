//
// Created by cpasjuste on 01/12/16.
//

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

SFMLTexture::SFMLTexture(const char *path) : Texture(path) {

    if (texture.loadFromFile(path)) {
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
        setTextureRect(IntRect(0, 0, (int) getSize().x, (int) getSize().y));
        sprite.setTexture(texture);
        pixels = new sf::Uint8[(int) getSize().x * (int) getSize().y * 2];
    } else {
        printf("Couldn't create texture: %s\n", path);
    }
}

SFMLTexture::SFMLTexture(const Vector2f &size) : Texture(size) {

    if (texture.create((uint) size.x, (uint) size.y)) {
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
        setTextureRect(IntRect(0, 0, (int) getSize().x, (int) getSize().y));
        sprite.setTexture(texture);
        pixels = new sf::Uint8[(int) getSize().x * (int) getSize().y * 2];
    } else {
        printf("Couldn't create texture\n");
    }
}

int SFMLTexture::lock(const FloatRect &rect, void **data, int *pitch) {

    *data = pixels;
    *pitch = (int) (getSize().x * 2);

    return 0;
}

void SFMLTexture::unlock() {

    GLint textureBinding;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
    sf::Texture::bind(&texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                    GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
    glBindTexture(GL_TEXTURE_2D, (GLuint) textureBinding);
}

void SFMLTexture::setFiltering(int filter) {

    texture.setSmooth((bool) filter);
}

SFMLTexture::~SFMLTexture() {

    if (pixels) {
        delete (pixels);
    }
}

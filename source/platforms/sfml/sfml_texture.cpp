//
// Created by cpasjuste on 01/12/16.
//

#include <GL/gl.h>
#include "cross2d/c2d.h"

using namespace c2d;

SFMLTexture::SFMLTexture(const char *path) : Texture(path) {

    if (texture.loadFromFile(path)) {
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
        setTextureRect(IntRect(0, 0, (int) getSize().x, (int) getSize().y));
        bpp = 4;
        pitch = (int) (getSize().x * bpp);
        pixels = new sf::Uint8[(int) getSize().x * (int) getSize().y * bpp];
        available = true;
    } else {
        printf("Couldn't create texture: %s\n", path);
    }
}

SFMLTexture::SFMLTexture(const Vector2f &size, int format) : Texture(size, format) {

    if (texture.create((uint) size.x, (uint) size.y)) {
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
        setTextureRect(IntRect(0, 0, (int) texture.getSize().x, (int) texture.getSize().y));
        bpp = 4;
        pitch = (int) (getSize().x * bpp);
        pixels = new sf::Uint8[(int) getSize().x * (int) getSize().y * bpp];
        available = true;
    } else {
        printf("Couldn't create texture\n");
    }
}

int SFMLTexture::lock(FloatRect *rect, void **pix, int *p) {

    if (!rect) {
        *pix = pixels;
    } else {
        *pix = (void *) (pixels + (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p) {
        *p = pitch;
    }

    return 0;
}

void SFMLTexture::unlock() {

    GLint textureBinding;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
    sf::Texture::bind(&texture);

    if (format == C2D_TEXTURE_FMT_RGBA8) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
    }

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

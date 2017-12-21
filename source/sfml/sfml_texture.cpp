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

/*
void SFMLTexture::Draw(int x, int y, int w, int h, float rotation) {

    sf::RenderStates states;
    SFMLRenderer *rdr = (SFMLRenderer *) renderer;

    // set sprite position
    sprite.setPosition(x, y);

    // set sprite scaling
    float scaleX = (float) w / (float) width;
    float scaleY = (float) h / (float) height;
    sprite.setScale(scaleX, scaleY);

    // set sprite rotation
    sf::Transform transform;
    transform.rotate(rotation, {(float) (x + w / 2), (float) (y + h / 2)});
    states.transform = transform;

    // set sprite shader
    sf::Shader *shader = (sf::Shader *) renderer->shaders->Get()->data;
    if (shader) {
        shader->setUniform("Texture", texture);
        shader->setUniform("MVPMatrix", sf::Glsl::Mat4(rdr->window.getView().getTransform().getMatrix()));
        shader->setUniform("TextureSize", sf::Glsl::Vec2(width, height));
        shader->setUniform("InputSize", sf::Glsl::Vec2(w, h));
        shader->setUniform("OutputSize", sf::Glsl::Vec2(w, h));
        states.shader = shader;
    }

    // draw sprite
    rdr->window.draw(sprite, states);
}
*/

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

//
// Created by cpasjuste on 01/12/16.
//

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

PSP2GLTexture::PSP2GLTexture(const char *path) : Texture(path) {

}

PSP2GLTexture::PSP2GLTexture(const Vector2f &size, int format) : Texture(size, format) {

}

int PSP2GLTexture::lock(FloatRect *rect, void **pix, int *p) {

    return 0;
}

void PSP2GLTexture::unlock() {

    /*
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
    */
}

void PSP2GLTexture::setFiltering(int filter) {

}

PSP2GLTexture::~PSP2GLTexture() {

}

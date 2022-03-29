//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL2__

#include "cross2d/c2d.h"
#include "cross2d/platforms/gl2/gl_texture.h"


using namespace c2d;

GLTexture::GLTexture(const std::string &path) : Texture(path) {
    available = createTexture() == 0;
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {
    available = createTexture() == 0;
}

GLTexture::GLTexture(const Vector2i &size, Texture::Format format) : Texture(size, format) {
    available = createTexture() == 0;
}

int GLTexture::createTexture() {
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    if (m_format == Format::RGBA8) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_tex_size.x, m_tex_size.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, m_tex_size.x, m_tex_size.y,
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_pixels);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

int GLTexture::lock(uint8_t **pixels, int *pitch, IntRect rect) {
    if (rect == IntRect()) {
        m_unlock_rect = {0, 0, getTextureRect().width, getTextureRect().height};
    } else {
        m_unlock_rect = rect;
    }

    if (*pixels) {
        *pixels = m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;
    }

    if (pitch) {
        *pitch = m_pitch;
    }

    return 0;
}

void GLTexture::unlock() {
    uint8_t *data = m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;

    printf("unlock: {%i, %i, %i, %i}\n",
           m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height);

    glBindTexture(GL_TEXTURE_2D, m_texID);

    if (m_unlock_rect.width > m_tex_size.x || m_unlock_rect.height > m_tex_size.y) {
        // resize texture requested from "lock"
        if (m_format == Format::RGBA8) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_unlock_rect.width, m_unlock_rect.height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, 0, 0, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, m_unlock_rect.width, m_unlock_rect.height,
                         0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        }
    } else {
        if (m_format == Format::RGBA8) {
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height,
                            GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height,
                            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::setFilter(Filter f) {
    Texture::setFilter(f);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture() {
    if (glIsTexture(m_texID)) {
        //printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &m_texID);
    }
}

#endif // __GL2__

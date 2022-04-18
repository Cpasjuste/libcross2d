//
// Created by cpasjuste on 01/12/16.
//

#if defined(__GL2__) || defined(__GL1__)

#include "cross2d/c2d.h"

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
    if (!m_pixels) {
        return -1;
    }

    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
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

int GLTexture::resize(const Vector2i &size, bool keepPixels) {
    printf("GLTexture::resize: %ix%i > %ix%i\n",
           m_tex_size.x, m_tex_size.y, (int) size.x, (int) size.y);

    if (size == m_tex_size) {
        printf("GLTexture::resize: size not changed, skipping...\n");
        return -1;
    }

    //auto dst_rect = IntRect(0, 0, size.x, size.y);
    auto dst_pitch = size.x * m_bpp;
    auto dst_pixels = (uint8_t *) malloc(dst_pitch * size.y);
    memset(dst_pixels, 0, dst_pitch * size.y);

    // copy pixels if requested
    if (keepPixels) {
        auto dst = dst_pixels;
        auto src = m_pixels;
        for (int i = 0; i < m_tex_size.y; i++) {
            memcpy(dst, src, m_pitch);
            src += m_pitch;
            dst += dst_pitch;
        }
    }

    // replace pixels buffer
    free(m_pixels);
    m_pixels = dst_pixels;

    // update texture
    glBindTexture(GL_TEXTURE_2D, m_texID);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    if (m_format == Format::RGBA8) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, 0, 0, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, size.x, size.y,
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_pixels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // update texture information
    m_pitch = dst_pitch;
    m_tex_size = m_image_size = {size.x, size.y};
    m_unlock_rect = {0, 0, size.x, size.y};
    setSize({(float) size.x, (float) size.y});
    setTextureRect(m_unlock_rect);
    setFilter(m_filter);

    return 0;
}

void GLTexture::unlock(int rowLength) {
    uint8_t *data = m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;

    //printf("GLTexture::unlock(%p): rect: {%i, %i, %i, %i}, pixels: %p\n",
    //     this, m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height, data);

    glBindTexture(GL_TEXTURE_2D, m_texID);

#ifndef GL_UNPACK_ROW_LENGTH
    IntRect rect = m_unlock_rect;
    if (rowLength > 0) {
        // fix missing GL_UNPACK_ROW_LENGTH support on ps4, used for fonts
        data = m_pixels;
        m_unlock_rect = {0, 0, m_tex_size.x, m_tex_size.y};
    }
#else
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
#endif
    if (m_format == Format::RGBA8) {
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height,
                        GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height,
                        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

#ifndef GL_UNPACK_ROW_LENGTH
    m_unlock_rect = rect;
#endif
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

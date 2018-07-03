//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL__

#include "c2d.h"
#include <GL/gl.h>
#include "skeleton/lodepng.h"

using namespace c2d;

GLTexture::GLTexture(const char *path) : Texture(path) {

    unsigned int w, h, error = 0;

    error = lodepng_decode32_file(&pixels, &w, &h, path);
    if (error) {
        printf("GLTexture::GLTexture: couldn't create texture:: %s\n", lodepng_error_text(error));
        return;
    }

    format = C2D_TEXTURE_FMT_RGBA8;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (texID) {
        setSize(Vector2f(w, h));
        setTextureRect(IntRect(0, 0, w, h));
        bpp = 4;
        pitch = (int) (getSize().x * bpp);
        available = true;
    } else {
        printf("GLTexture::GLTexture: couldn't create texture: %s\n", path);
    }

    printf("GLTexture(%p): pixels=%p\n", this, (void *) pixels);
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    unsigned int w, h, error = 0;

    error = lodepng_decode32(&pixels, &w, &h, buffer, (size_t) bufferSize);
    if (error) {
        printf("GLTexture::GLTexture: couldn't create texture: %s\n", lodepng_error_text(error));
        return;
    }

    format = C2D_TEXTURE_FMT_RGBA8;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (texID) {
        setSize(Vector2f(w, h));
        setTextureRect(IntRect(0, 0, w, h));
        bpp = 4;
        pitch = (int) (getSize().x * bpp);
        available = true;
    } else {
        printf("GLTexture::GLTexture: couldn't create texture: %s\n", path);
    }

    printf("GLTexture(%p)\n", this);
}

GLTexture::GLTexture(const Vector2f &size, int format) : Texture(size, format) {

    glGenTextures(1, &texID);
    if (texID) {
        pixels = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
        glBindTexture(GL_TEXTURE_2D, texID);
        switch (format) {
            case C2D_TEXTURE_FMT_RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
            case C2D_TEXTURE_FMT_ARGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
                break;
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        available = true;
    } else {
        printf("GLTexture::GLTexture: couldn't create texture");
    }

    printf("GLTexture(%p)\n", this);
}

int GLTexture::resize(const Vector2f &size, bool copyPixels) {

    printf("GLTexture::resize: %i x %i\n", (int) size.x, (int) size.y);

    if (size.x == getSize().x && size.y == getSize().y) {
        return -1;
    }

    // copy pixels
    unsigned char *dst = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
    pitch = (int) (size.x * bpp);

    if (copyPixels) {

        Vector2i dst_size = Vector2i(
                std::min((int) getSize().x, (int) size.x),
                std::min((int) getSize().y, (int) size.y));

        // TODO: crash here, why ?
        for (int i = 0; i < dst_size.y; i++) {
            memcpy(dst, pixels, (size_t) dst_size.x * bpp);
            pixels += dst_size.x * bpp;
            dst += pitch;
        }
    }

    free(pixels);
    pixels = dst;

    glDeleteTextures(1, &texID);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    switch (format) {
        case C2D_TEXTURE_FMT_RGBA8:
            glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
        case C2D_TEXTURE_FMT_ARGB8:
            glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            break;
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
            break;
    }
    setSize(size);
    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFiltering(filtering);

    printf("GLTexture::resize\n");

    return -1;
}

int GLTexture::lock(FloatRect *rect, void **pix, int *p) {

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

void GLTexture::unlock() {

    glBindTexture(GL_TEXTURE_2D, texID);

    switch (format) {
        case C2D_TEXTURE_FMT_RGBA8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
        case C2D_TEXTURE_FMT_ARGB8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            break;
        default:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
            break;
    }
}

void GLTexture::setFiltering(int filter) {

    filtering = filter;

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    filtering == C2D_TEXTURE_FILTER_LINEAR ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    filtering == C2D_TEXTURE_FILTER_LINEAR ? GL_LINEAR : GL_NEAREST);
}

GLTexture::~GLTexture() {

    printf("~GLTexture(%p): pixels=%p\n", this, (void *) pixels);
    if (pixels) {
        free(pixels);
        pixels = nullptr;
    }

    if (texID) {
        printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
    }
}

#endif // __GL__

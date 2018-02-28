//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL__

#include <png.h>
#include "c2d.h"
#include "skeleton/lodepng.h"

#ifdef __TINYGL__

#include "pTinyGL/pgl.h"

#define GL_UNSIGNED_SHORT_5_6_5 GL_UNSIGNED_BYTE
#else

#include "GL/gl.h"

#endif

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

    printf("GLTexture(%p)\n", this);
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
        glTexImage2D(GL_TEXTURE_2D, 0, bpp == 4 ? 4 : 3, (GLsizei) size.x, (GLsizei) size.y, 0,
                     bpp == 4 ? GL_RGBA : GL_RGB, bpp == 4 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT_5_6_5, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        available = true;
    } else {
        printf("GLTexture::GLTexture: couldn't create texture");
    }

    printf("GLTexture(%p)\n", this);
}

int GLTexture::resize(const Vector2f &size, bool copyPixels) {

    // TODO
    /*
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
    glTexImage2D(GL_TEXTURE_2D, 0, bpp == 4 ? 4 : 3, (GLsizei) size.x, (GLsizei) size.y, 0,
                 bpp == 4 ? GL_RGBA : GL_RGB, bpp == 4 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT_5_6_5, pixels);

    setSize(size);
    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFiltering(filtering);

    printf("GLTexture::resize\n");
    */

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

#ifdef __TINYGL__
    // TODO: implement glTexSubImage2D in pTinyGL
    glDeleteTextures(1, &texID);
    glGenTextures(1, &texID);
    if (texID) {
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, bpp == 4 ? 4 : 3, (GLsizei) getSize().x, (GLsizei) getSize().y, 0,
                     bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        filtering == C2D_TEXTURE_FILTER_LINEAR ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        filtering == C2D_TEXTURE_FILTER_LINEAR ? GL_LINEAR : GL_NEAREST);
    }
#else
    glBindTexture(GL_TEXTURE_2D, texID);

    if (format == C2D_TEXTURE_FMT_RGBA8) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
    }
#endif
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

    printf("~GLTexture(%p)\n", this);
    if (pixels) {
        printf("free(pixels)\n");
        free(pixels);
    }

    if (texID) {
        printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
    }
}

#endif // __GL__

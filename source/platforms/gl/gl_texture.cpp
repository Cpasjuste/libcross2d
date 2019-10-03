//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL__

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

GLTexture::GLTexture(const std::string &p) : Texture(p) {

    int w, h, n = 0;

    pixels = stbi_load(path.c_str(), &w, &h, &n, 4);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID) {
        pitch = w * bpp;
        setSize((float) w, (float) h);
        setTextureRect(IntRect(0, 0, w, h));
        setTexture(this, true);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, path.c_str());
    }

    //printf("GLTexture(%p): %ix%i\n", this, w, h);
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    int w, h, n = 0;

    pixels = stbi_load_from_memory(buffer, bufferSize, &w, &h, &n, 4);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }

    GL_CHECK(glGenTextures(1, &texID));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, texID));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID) {
        pitch = w * bpp;
        setSize((float) w, (float) h);
        setTextureRect(IntRect(0, 0, w, h));
        setTexture(this, true);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture\n", this);
    }

    //printf("GLTexture(%p): %ix%i\n", this, w, h);
}

GLTexture::GLTexture(const Vector2f &size, Format format) : Texture(size, format) {

    glGenTextures(1, &texID);

    if (texID) {
        pixels = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
        glBindTexture(GL_TEXTURE_2D, texID);

        switch (format) {
            case Format::RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
#ifndef __SDL2_GLES__
            case Format::ARGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);
                break;
            case Format::BGRA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
                break;
            case Format::ABGR8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, pixels);
                break;
#endif
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        setSize(size);
        setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
        setTexture(this, true);
        glBindTexture(GL_TEXTURE_2D, 0);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture", this);
    }

    //printf("GLTexture(%p): %ix%i\n", this, (int) size.x, (int) size.y);
}

#if 0
int GLTexture::resize(const Vector2f &size, bool copyPixels) {

    printf("GLTexture::resize: %i x %i\n", (int) size.x, (int) size.y);

    if (size.x == getTextureRect().width && size.y == getTextureRect().height) {
        return -1;
    }

    // copy pixels
    unsigned char *dst = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
    pitch = (int) (size.x * bpp);

    if (copyPixels) {

        Vector2i dst_size = Vector2i(
                std::min(getTextureRect().width, (int) size.x),
                std::min(getTextureRect().height, (int) size.y));

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

    setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
    setFiltering(filtering);

    printf("GLTexture::resize\n");

    return -1;
}
#endif

int GLTexture::save(const std::string &path) {

    int res;
    int width = getTextureRect().width;
    int height = getTextureRect().height;

    if (!pixels) {
        return -1;
    }

    if (bpp == 2) {
        // convert rgb565 to bgr888
        auto *tmp = (unsigned char *) malloc((size_t) width * (size_t) height * 3);
        for (int i = 0; i < width * height; i++) {
            signed short nColour = ((signed short *) pixels)[i];
            *(tmp + i * 3 + 2) = (unsigned char) ((nColour & 0x1F) << 3);
            *(tmp + i * 3 + 2) |= *(tmp + 3 * i + 0) >> 5;
            *(tmp + i * 3 + 1) = (unsigned char) (((nColour >> 5) & 0x3F) << 2);
            *(tmp + i * 3 + 1) |= *(tmp + i * 3 + 1) >> 6;
            *(tmp + i * 3 + 0) = (unsigned char) (((nColour >> 11) & 0x1F) << 3);
            *(tmp + i * 3 + 0) |= *(tmp + i * 3 + 2) >> 5;
        }
        res = stbi_write_png(path.c_str(), width, height, 3, tmp, width * 3);
        free(tmp);
    } else {
        res = stbi_write_png(path.c_str(), width, height, 4, pixels, width * 4);
    }

    return res;
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
        case Format::RGBA8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
#ifndef __SDL2_GLES__
        case Format::BGRA8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            break;
        case Format::ARGB8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);
            break;
        case Format::ABGR8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, pixels);
            break;
#endif
        default:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
            break;
    }
}

void GLTexture::setFilter(Filter f) {

    this->filter = f;

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
}

void GLTexture::setShader(int shaderIndex) {

    ShaderList *shaderList = c2d_renderer->getShaderList();
    if (shaderIndex >= shaderList->getCount()) {
        shader = shaderList->get(0);
        return;
    }

    shader = shaderList->get(shaderIndex);
}

GLTexture::~GLTexture() {

    //printf("~GLTexture(%p)\n", this);

    if (pixels) {
        stbi_image_free(pixels);
        pixels = nullptr;
    }

    if (glIsTexture(texID) == GL_TRUE) {
        //printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}

#endif // __GL__

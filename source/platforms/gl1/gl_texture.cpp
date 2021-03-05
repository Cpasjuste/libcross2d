//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL1__

#include <cross2d/platforms/gl1/gl_texture.h>

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define GL_ABGR_EXT 0x8000

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

// TODO: dreamcast: fix pow2 "setOrigin"
#ifdef __DREAMCAST__

int GLTexture::pow2(int w) {
    if (w == 0)
        return 0;
    int n = 2;
    while (w > n)
        n <<= 1;
    return n;
}

stbi_uc *GLTexture::getPixels(int *w, int *h, const unsigned char *buffer, int bufferSize) {

    int n = 0;
    stbi_uc *img;

    if (buffer) {
        img = stbi_load_from_memory(buffer, bufferSize, w, h, &n, 4);
    } else {
        img = stbi_load(path.c_str(), w, h, &n, bpp);
        if (img == nullptr) {
            return nullptr;
        }
    }

    // copy img to power of 2 pixel data
    tex_size.x = pow2(*w), tex_size.y = pow2(*h);
    pixels = (stbi_uc *) malloc((size_t) (tex_size.x * tex_size.y * bpp));
    if (pixels == nullptr) {
        free(img);
        return nullptr;
    }

    memset(pixels, 0, tex_size.x * tex_size.y * bpp);
    stbi_uc *dst = pixels;
    int dst_pitch = tex_size.x * bpp;
    stbi_uc *src = img;
    int src_pitch = *w * bpp;
    for (int i = 0; i < *h; i++) {
        memcpy(dst, src, (size_t) *w * bpp);
        dst += dst_pitch;
        src += src_pitch;
    }

    free(img);
    return pixels;
}

#endif

GLTexture::GLTexture(const std::string &p) : Texture(p) {

    int w, h;

#ifdef __DREAMCAST__
    pixels = getPixels(&w, &h);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }
#else
    int n = 0;
    pixels = stbi_load(path.c_str(), &w, &h, &n, 4);
    tex_size = {w, h};
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }
#endif

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_size.x, tex_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID != 0u) {
        pitch = (int) tex_size.x * bpp;
        Texture::setSize((float) w, (float) h);
        setTexture(this, true);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, path.c_str());
    }

    printf("GLTexture(%p): %i x %i (id=%i)\n", this, tex_size.x, tex_size.y, texID);
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    int w, h;

#ifdef __DREAMCAST__
    pixels = getPixels(&w, &h, buffer, bufferSize);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture \n", this);
        return;
    }
#else
    int n = 0;
    pixels = stbi_load_from_memory(buffer, bufferSize, &w, &h, &n, 4);
    tex_size = {w, h};
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }
#endif

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_size.x, tex_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID != 0u) {
        pitch = tex_size.x * bpp;
        Texture::setSize((float) w, (float) h);
        setTexture(this, true);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture\n", this);
    }

    printf("GLTexture(%p): %i x %i (id=%i)\n", this, tex_size.x, tex_size.y, texID);
}

GLTexture::GLTexture(const Vector2f &size, Format format) : Texture(size, format) {

    glGenTextures(1, &texID);

#ifdef __DREAMCAST__
    tex_size.x = pow2((int) size.x);
    tex_size.y = pow2((int) size.y);
#else
    tex_size = {size.x, size.y};
#endif

    if (texID != 0u) {
        pitch = tex_size.x * bpp;
        pixels = (unsigned char *) malloc(tex_size.x * tex_size.y * bpp);
        glBindTexture(GL_TEXTURE_2D, texID);

        switch (format) {
            case Format::RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture::setSize(size);
        setTexture(this, true);
        glBindTexture(GL_TEXTURE_2D, 0);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture", this);
    }

    printf("GLTexture(%p): %i x %i (id=%i)\n", this, tex_size.x, tex_size.y, texID);
}

int GLTexture::save(const std::string &path) {

    int res;
    int width = getTextureRect().width;
    int height = getTextureRect().height;

    if (pixels == nullptr) {
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

    if (rect == nullptr) {
        *pix = pixels;
    } else {
        *pix = (void *) (pixels + (int) rect->top * pitch + (int) rect->left * bpp);
    }

    if (p != nullptr) {
        *p = pitch;
    }

    return 0;
}

void GLTexture::unlock(void *data) {

    glBindTexture(GL_TEXTURE_2D, texID);

    switch (format) {
#ifdef __DREAMCAST__
        case Format::RGBA8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                         (GLsizei) tex_size.x, (GLsizei) tex_size.y,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, data ? data : pixels);
            break;
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                         (GLsizei) tex_size.x, (GLsizei) tex_size.y,
                         0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5_TWID_KOS, data ? data : pixels);
            break;
#else
            case Format::RGBA8:
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                (GLsizei) getTextureRect().left, (GLsizei) getTextureRect().top,
                                (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                                GL_RGBA, GL_UNSIGNED_BYTE, data ? data : pixels);
                break;
            default:
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                (GLsizei) getTextureRect().left, (GLsizei) getTextureRect().top,
                                (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                                GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data ? data : pixels);
                break;
#endif
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::setFilter(Filter f) {

    this->filter = f;

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture() {

    printf("~GLTexture(%p)\n", this);

    if (texID > 0) {
        //printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
        texID = 0;
    }

    if (pixels != nullptr) {
        free(pixels);
        pixels = nullptr;
    }
}

#endif // __GL1__

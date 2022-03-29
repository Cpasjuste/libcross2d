//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL2__

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define GL_ABGR_EXT 0x8000

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

#if defined(__POW2_TEX__)

static int pow2(int w) {
    if (w == 0)
        return 0;
    int n = 2;
    while (w > n)
        n <<= 1;
    return n;
}

#endif

unsigned char *GLTexture::getPixels(int *w, int *h, const unsigned char *buffer, int bufferSize) {
    int n = 0;
    stbi_uc *img;

    if (buffer) {
        img = stbi_load_from_memory(buffer, bufferSize, w, h, &n, 4);
        if (!img) {
            return nullptr;
        }
    } else {
        img = stbi_load(path.c_str(), w, h, &n, bpp);
        if (!img) {
            return nullptr;
        }
    }

    pitch = *w * bpp;
#ifndef __POW2_TEX__
    pixels = img;
    tex_size = {*w, *h};
#else
    // copy img to power of 2 pixel data
    tex_size.x = pow2(*w), tex_size.y = pow2(*h);
    pixels = (unsigned char *) malloc((size_t) (tex_size.x * tex_size.y * bpp));
    if (!pixels) {
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
#endif

    return pixels;
}

GLTexture::GLTexture(const std::string &p) : Texture(p) {
    int w, h;

    pixels = getPixels(&w, &h);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture (%s)\n", this, path.c_str());
        return;
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_size.x, tex_size.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID) {
        setTexture(this);
        Texture::setSize((float) w, (float) h);
        Texture::setTextureRect({0, 0, w, h});
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, path.c_str());
    }

    printf("GLTexture(%p): size: %ix%i, bpp: %i, pitch: %i (tex_size: %ix%i)\n",
           this, w, h, bpp, pitch, tex_size.x, tex_size.y);
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {
    int w, h;

    pixels = getPixels(&w, &h, buffer, bufferSize);
    if (!pixels) {
        printf("GLTexture(%p): couldn't create texture from buffer\n", this);
        return;
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_size.x, tex_size.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID) {
        setTexture(this);
        Texture::setSize((float) w, (float) h);
        Texture::setTextureRect({0, 0, w, h});
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture\n", this);
    }

    printf("GLTexture(%p): size: %ix%i, bpp: %i, pitch: %i (tex_size: %ix%i)\n",
           this, w, h, bpp, pitch, tex_size.x, tex_size.y);
}

GLTexture::GLTexture(const Vector2f &size, Format format) : Texture(size, format) {
    glGenTextures(1, &texID);

    // on ps vita it's faster to "unlock" power of 2 textures (glTexSubImage2D)
#if defined(__POW2_TEX__)
    tex_size = {pow2((int) size.x), pow2((int) size.y)};
#else
    tex_size = {(int) size.x, (int) size.y};
#endif

    if (texID) {
        pixels = (unsigned char *) malloc(((int) (tex_size.x * tex_size.y) * bpp));
        glBindTexture(GL_TEXTURE_2D, texID);

        switch (format) {
            case Format::RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
#ifndef __GLES2__
            case Format::ARGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);
                break;
            case Format::BGRA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
                break;
            case Format::ABGR8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, pixels);
                break;
#endif
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) tex_size.x, (GLsizei) tex_size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        setTexture(this);
        Texture::setSize(size);
        Texture::setTextureRect({0, 0, (int) size.x, (int) size.y});
        glBindTexture(GL_TEXTURE_2D, 0);
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture", this);
    }

    printf("GLTexture(%p): size: %ix%i, bpp: %i, pitch: %i (tex_size: %ix%i)\n",
           this, (int) size.x, (int) size.y, bpp, pitch, tex_size.x, tex_size.y);
}

int GLTexture::resize(const Vector2i &size, bool keepPixels) {
    printf("GLTexture::resize: %ix%i > %ix%i\n",
           tex_size.x, tex_size.y, (int) size.x, (int) size.y);

    if (size.x == getTextureRect().width && size.y == getTextureRect().height) {
        return -1;
    }

    pitch = (int) (size.x * bpp);
    auto new_pixels = (unsigned char *) malloc((ssize_t) (pitch * size.y));

    // copy pixels if requested
    if (keepPixels) {
        Vector2i dst_size = Vector2i(
                std::min((int) getSize().x, (int) size.x),
                std::min((int) getSize().y, (int) size.y));
        auto src = pixels;
        auto dst = new_pixels;
        for (int i = 0; i < dst_size.y; i++) {
            memcpy(dst, src, (ssize_t) (dst_size.x * bpp));
            src += dst_size.x * bpp;
            dst += pitch;
        }
    }

    free(pixels);
    pixels = new_pixels;

    glBindTexture(GL_TEXTURE_2D, texID);

    switch (format) {
        case Format::RGBA8:
            // invalidate texture storage
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
#ifndef __GLES2__
        case Format::ARGB8:
            // invalidate texture storage
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);
            break;
        case Format::BGRA8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            break;
        case Format::ABGR8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, pixels);
            break;
#endif
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, 0, 0, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) size.x, (GLsizei) size.y, 0,
                         GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
            break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    tex_size = {size.x, size.y};
    setTexture(this);
    Texture::setSize({(float) size.x, (float) size.y});
    Texture::setTextureRect({0, 0, (int) size.x, (int) size.y});

    setFilter(filter);

    printf("GLTexture::resize: done\n");

    return 0;
}

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

int GLTexture::lock(IntRect *rect, uint8_t **pix, int *p) {
    if (!rect) {
        *pix = pixels;
        unlock_rect = {0, 0, getTextureRect().width, getTextureRect().height};
    } else {
        *pix = pixels + rect->top * pitch + rect->left * bpp;
        unlock_rect = *rect;
    }

    if (p != nullptr) {
        *p = pitch;
    }

    return 0;
}

void GLTexture::unlock() {
    uint8_t *data = pixels;
    if (unlock_rect.top != 0 || unlock_rect.left != 0) {
        data = pixels + unlock_rect.top * pitch + unlock_rect.left * bpp;
    }

    printf("unlock: {%i, %i, %i, %i}\n",
           unlock_rect.left, unlock_rect.top, unlock_rect.width, unlock_rect.height);

    glBindTexture(GL_TEXTURE_2D, texID);

    switch (format) {
        case Format::RGBA8:
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            unlock_rect.left, unlock_rect.top,
                            (GLsizei) unlock_rect.width, (GLsizei) unlock_rect.height,
                            GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
#ifndef __GLES2__
        case Format::BGRA8:
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            unlock_rect.left, unlock_rect.top,
                            (GLsizei) unlock_rect.width, (GLsizei) unlock_rect.height,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
            break;
        case Format::ARGB8:
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            unlock_rect.left, unlock_rect.top,
                            (GLsizei) unlock_rect.width, (GLsizei) unlock_rect.height,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, data);
            break;
        case Format::ABGR8:
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            unlock_rect.left, unlock_rect.top,
                            (GLsizei) unlock_rect.width, (GLsizei) unlock_rect.height,
                            GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, data);
            break;
#endif
        default:
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            unlock_rect.left, unlock_rect.top,
                            (GLsizei) unlock_rect.width, (GLsizei) unlock_rect.height,
                            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            break;
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
    //printf("~GLTexture(%p)\n", this);
    if (pixels) {
        stbi_image_free(pixels);
    }

    if (glIsTexture(texID) == GL_TRUE) {
        //printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}

#endif // __GL2__

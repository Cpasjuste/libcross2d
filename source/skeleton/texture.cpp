//
// Created by cpasjuste on 29/11/17.
//

#include "cross2d/c2d.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "cross2d/skeleton/stb_image.h"
#include "cross2d/skeleton/stb_image_write.h"

using namespace c2d;

Texture::Texture(const std::string &p) : RectangleShape({0, 0}) {
    int w, h, n;
    type = Type::Texture;
    m_format = Format::RGBA8;
    m_bpp = 4;
    m_path = p;

    // load pixels buffer
    m_pixels = stbi_load(m_path.c_str(), &w, &h, &n, m_bpp);
    if (!m_pixels) {
        return;
    }

    // set texture parameters
    m_pitch = w * m_bpp;
    m_tex_size = {w, h};
    m_unlock_rect = {0, 0, w, h};
    Texture::setSize((float) w, (float) h);
    Texture::setTexture(this);
    Texture::setTextureRect({0, 0, w, h});
    Texture::setFillColor(Color::White);
    Texture::setShader(0);

    printf("Texture(%p): size: %ix%i, bpp: %i, pitch: %i, path: %s\n",
           this, m_tex_size.x, m_tex_size.y, m_bpp, m_pitch, m_path.c_str());
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : RectangleShape({0, 0}) {
    int w, h, n;
    type = Type::Texture;
    m_format = Format::RGBA8;
    m_bpp = 4;

    // load pixels buffer
    m_pixels = stbi_load_from_memory(buffer, bufferSize, &w, &h, &n, m_bpp);
    if (!m_pixels) {
        return;
    }

    // set texture parameters
    m_pitch = w * m_bpp;
    m_tex_size = {w, h};
    m_unlock_rect = {0, 0, w, h};
    Texture::setSize((float) w, (float) h);
    Texture::setTexture(this);
    Texture::setTextureRect({0, 0, w, h});
    Texture::setFillColor(Color::White);
    Texture::setShader(0);

    printf("Texture(%p): size: %ix%i, bpp: %i, pitch: %i\n",
           this, m_tex_size.x, m_tex_size.y, m_bpp, m_pitch);
}

Texture::Texture(const Vector2i &size, Format format) : RectangleShape(Vector2f{0, 0}) {
    type = Type::Texture;
    m_format = format;
    m_bpp = format == Format::RGB565 ? 2 : 4;

    m_pixels = (uint8_t *) malloc(size.x * size.y * m_bpp);
    if (!m_pixels) {
        return;
    }

    memset(m_pixels, 0, size.x * size.y * m_bpp);

    // set texture parameters
    m_pitch = size.x * m_bpp;
    m_tex_size = {size.x, size.y};
    m_unlock_rect = {0, 0, size.x, size.y};
    Texture::setSize((float) size.x, (float) size.y);
    Texture::setTexture(this);
    Texture::setTextureRect({0, 0, size.x, size.y});
    Texture::setFillColor(Color::White);
    Texture::setShader(0);

    printf("Texture(%p): size: %ix%i, bpp: %i, pitch: %i\n",
           this, m_tex_size.x, m_tex_size.y, m_bpp, m_pitch);
}

int Texture::lock(uint8_t **pixels, int *pitch, IntRect rect) {
    if (rect == IntRect()) {
        m_unlock_rect = {0, 0, getTextureRect().width, getTextureRect().height};
    } else {
        m_unlock_rect = rect;
    }

    if (pitch) {
        *pitch = m_pitch;
    }

    *pixels = m_pixels + m_unlock_rect.top * m_pitch + m_unlock_rect.left * m_bpp;

    //printf("Texture::lock(%p): rect: {%i, %i, %i, %i}, pixels: %p\n",
    //     this, m_unlock_rect.left, m_unlock_rect.top, m_unlock_rect.width, m_unlock_rect.height, *pixels);

    return 0;
}

int Texture::resize(const Vector2i &size, bool keepPixels) {
    printf("Texture::resize: %ix%i > %ix%i\n",
           m_tex_size.x, m_tex_size.y, (int) size.x, (int) size.y);

    if (size == m_tex_size || size.x < m_tex_size.x || size.y < m_tex_size.y) {
        printf("Texture::resize: invalid parameters\n");
        return -1;
    }

    auto dst_rect = IntRect(0, 0, size.x, size.y);
    auto dst_pitch = size.x * m_bpp;
    auto dst_pixels = (uint8_t *) malloc(dst_pitch * size.y);
    memset(dst_pixels, 0, dst_pitch * size.y);

    uint8_t *src_pixels;
    int src_pitch;
    auto src_rect = IntRect(0, 0, m_tex_size.x, m_tex_size.y);
    lock(&src_pixels, &src_pitch, dst_rect);

    // copy pixels if requested
    if (keepPixels) {
        auto dst = dst_pixels;
        for (int i = 0; i < src_rect.height; i++) {
            memcpy(dst, src_pixels, src_pitch);
            src_pixels += src_pitch;
            dst += dst_pitch;
        }
    }

    // replace pixels buffer
    free(m_pixels);
    m_pixels = dst_pixels;

    // upload pixels (and resize if needed)
    unlock();

    // update texture information
    m_pitch = dst_pitch;
    m_tex_size = {dst_rect.width, dst_rect.height};
    setSize({(float) dst_rect.width, (float) dst_rect.height});
    setTextureRect(dst_rect);
    setFilter(m_filter);

    return 0;
}

int Texture::save(const std::string &path) {
    int res;
    int width = getTextureRect().width;
    int height = getTextureRect().height;

    printf("Texture::save(%p): %ix%i, bpp: %i, pitch: %i\n",
           this, width, height, m_bpp, m_pitch);

    if (!m_pixels) {
        return -1;
    }

    if (m_bpp == 2) {
        // convert rgb565 to bgr888
        auto *tmp = (unsigned char *) malloc((size_t) width * (size_t) height * 3);
        for (int i = 0; i < width * height; i++) {
            signed short nColour = ((signed short *) m_pixels)[i];
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
        res = stbi_write_png(path.c_str(), width, height, m_bpp, m_pixels, m_pitch);
    }

    return res;
}

void Texture::setShader(int shaderIndex) {
    ShaderList *shaderList = c2d_renderer->getShaderList();
    m_shader = shaderList->get(shaderIndex);
    if (!m_shader) {
        printf("Texture::setShader: shader not found (%i)\n", shaderIndex);
        m_shader = shaderList->get(0);
    }
}

void Texture::setShader(const std::string &shaderName) {
    ShaderList *shaderList = c2d_renderer->getShaderList();
    m_shader = shaderList->get(shaderName);
    if (!m_shader) {
        printf("Texture::setShader: shader not found (%s)\n", shaderName.c_str());
        m_shader = shaderList->get(0);
    }
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
    if (m_pixels) {
        free(m_pixels);
    }
}


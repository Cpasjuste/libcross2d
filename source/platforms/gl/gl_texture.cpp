//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL__

#include <png.h>
#include "c2d.h"
#include "skeleton/lodepng.h"

using namespace c2d;

GLTexture::GLTexture(const char *path) : Texture(path) {

    unsigned int w, h, error = 0;

    error = lodepng_decode32_file(&pixels, &w, &h, path);
    if (error) {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, lodepng_error_text(error));
        return;
    }

    format = C2D_TEXTURE_FMT_RGBA8;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (texID) {
        bpp = 4;
        setTextureRect(IntRect(0, 0, w, h));
        pitch = getTextureRect().width * bpp;
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, path);
    }

    printf("GLTexture(%p): %ix%i\n", this, w, h);
}

GLTexture::GLTexture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    unsigned int w, h, error = 0;

    error = lodepng_decode32(&pixels, &w, &h, buffer, (size_t) bufferSize);
    if (error) {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, lodepng_error_text(error));
        return;
    }

    format = C2D_TEXTURE_FMT_RGBA8;

    GL_CHECK(glGenTextures(1, &texID));
    //GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, texID));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                          GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    if (texID) {
        bpp = 4;
        setTextureRect(IntRect(0, 0, w, h));
        pitch = getTextureRect().width * bpp;
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture: %s\n", this, path);
    }

    printf("GLTexture(%p): %ix%i\n", this, w, h);
}

GLTexture::GLTexture(const Vector2f &size, int format) : Texture(size, format) {

    // TODO: fix internal format
    glGenTextures(1, &texID);
    if (texID) {
        pixels = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
        glBindTexture(GL_TEXTURE_2D, texID);
        switch (format) {
            case C2D_TEXTURE_FMT_RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
            case C2D_TEXTURE_FMT_ARGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
                break;
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        setTextureRect(IntRect(0, 0, (int) size.x, (int) size.y));
        available = true;
    } else {
        printf("GLTexture(%p): couldn't create texture", this);
    }

    printf("GLTexture(%p): %ix%i\n", this, (int) size.x, (int) size.y);
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

int GLTexture::save(const char *path) {

    unsigned char *converted = nullptr;
    png_bytep *rows = nullptr;

    const char *szAuthor = "Cpasjuste";
    const char *szDescription = "Screenshot";
    const char *szCopyright = "Cpasjuste";
    const char *szSoftware = "libcross2d @ libpng";
    const char *szSource = "libcross2d";

    FILE *ff = nullptr;
    png_text text_ptr[7];
    int num_text = 7;
    time_t currentTime;
    png_time_struct png_time_now;

    if (!pixels) {
        return -1;
    }

    int w = getTextureRect().width, h = getTextureRect().height;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)
                nullptr);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(ff);
        remove(path);
        return -1;
    }

    // Convert the image to 32-bit
    if (bpp < 4) {
        unsigned char *pTemp = (unsigned char *) malloc(w * h * sizeof(int));
        if (bpp == 2) {
            for (int i = 0; i < h * w; i++) {
                signed short nColour = ((signed short *) pixels)[i];
                // Red
                *(pTemp + i * 4 + 0) = (unsigned char) ((nColour & 0x1F) << 3);
                *(pTemp + i * 4 + 0) |= *(pTemp + 4 * i + 0) >> 5;
                // Green
                *(pTemp + i * 4 + 1) = (unsigned char) (((nColour >> 5) & 0x3F) << 2);
                *(pTemp + i * 4 + 1) |= *(pTemp + i * 4 + 1) >> 6;
                // Blue
                *(pTemp + i * 4 + 2) = (unsigned char) (((nColour >> 11) & 0x1F) << 3);
                *(pTemp + i * 4 + 2) |= *(pTemp + i * 4 + 2) >> 5;
            }
        } else {
            memset(pTemp, 0, w * h * sizeof(int));
            for (int i = 0; i < h * w; i++) {
                *(pTemp + i * 4 + 0) = *(pixels + i * 3 + 0);
                *(pTemp + i * 4 + 1) = *(pixels + i * 3 + 1);
                *(pTemp + i * 4 + 2) = *(pixels + i * 3 + 2);
            }
        }
        converted = pTemp;
    }

    // Get the time
    time(&currentTime);
    png_convert_from_time_t(&png_time_now, currentTime);

    ff = fopen(path, "wb");
    if (ff == nullptr) {
        printf("C2DUINXVideo::save: fopen failed: `%s`\n", path);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        if (converted) {
            free(converted);
        }
        return -1;
    }

    // Fill the PNG text fields
    text_ptr[0].key = (png_charp) "Title";
    text_ptr[0].text = (png_charp) "ROM";
    text_ptr[1].key = (png_charp) "Author";
    text_ptr[1].text = (png_charp) szAuthor;
    text_ptr[2].key = (png_charp) "Description";
    text_ptr[2].text = (png_charp) szDescription;
    text_ptr[3].key = (png_charp) "Copyright";
    text_ptr[3].text = (png_charp) szCopyright;
    text_ptr[4].key = (png_charp) "Software";
    text_ptr[4].text = (png_charp) szSoftware;
    text_ptr[5].key = (png_charp) "Source";
    text_ptr[5].text = (png_charp) szSource;
    text_ptr[6].key = (png_charp) "Comment";
    text_ptr[6].text = (png_charp) "Created by running the game in an emulator";
    for (int i = 0; i < num_text; i++) {
        text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    }
    png_set_text(png_ptr, info_ptr, text_ptr, num_text);
    png_init_io(png_ptr, ff);
    png_set_IHDR(png_ptr, info_ptr, (png_uint_32) w, (png_uint_32) h, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
    png_set_bgr(png_ptr);
    rows = (png_bytep *) malloc(h * sizeof(png_bytep));
    for (int y = 0; y < h; y++) {
        rows[y] = converted + (y * w * sizeof(int));
    }
    png_write_image(png_ptr, rows);
    png_write_end(png_ptr, info_ptr);
    if (rows) {
        free(rows);
    }
    fclose(ff);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    if (converted) {
        free(converted);
    }

    return 0;
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
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
        case C2D_TEXTURE_FMT_ARGB8:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            break;
        default:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            (GLsizei) getTextureRect().width, (GLsizei) getTextureRect().height,
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

void GLTexture::setShader(int shaderIndex) {

    ShaderList *shaderList = c2d_renderer->getShaderList();
    if (shaderIndex >= shaderList->getCount()) {
        shader = shaderList->get(0);
        return;
    }

    shader = shaderList->get(shaderIndex);
}

GLTexture::~GLTexture() {

    printf("~GLTexture(%p)\n", this);

    if (pixels) {
        free(pixels);
        pixels = nullptr;
    }

    if (texID) {
        printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}

#endif // __GL__

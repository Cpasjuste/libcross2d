//
// Created by cpasjuste on 01/12/16.
//

#include <png.h>
#include "c2d.h"
#include "skeleton/lodepng.h"

#ifdef __TINYGL__

#include "gl.h"
//#include "skeleton/TinyGL/GL/tinygl.h"

#else

#include "GL/gl.h"

#endif

using namespace c2d;

GLTexture::GLTexture(const char *path) : Texture(path) {

    unsigned int w, h, error = 0;

    error = lodepng_decode32_file(&pixels, &w, &h, path);
    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
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
        printf("Couldn't create texture: %s\n", path);
    }
}

GLTexture::GLTexture(const Vector2f &size, int format) : Texture(size, format) {

    glGenTextures(1, &texID);
    if (texID) {
        pixels = (unsigned char *) malloc((size_t) (size.x * size.y * bpp));
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, bpp == 4 ? 4 : 3, (GLsizei) size.x, (GLsizei) size.y, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        available = true;
    } else {
        printf("Couldn't create texture: %s\n", path);
    }
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

    if (format == C2D_TEXTURE_FMT_RGBA8) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) getSize().x, (GLsizei) getSize().y,
                        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
    }
}

void GLTexture::setFiltering(int filter) {

}

GLTexture::~GLTexture() {

    if (pixels) {
        free(pixels);
        glDeleteTextures(1, &texID);
    }
}

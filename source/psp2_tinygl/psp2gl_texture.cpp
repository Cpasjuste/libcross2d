//
// Created by cpasjuste on 01/12/16.
//

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

PSP2GLTexture::PSP2GLTexture(const char *path) : Texture(path) {

}

PSP2GLTexture::PSP2GLTexture(const Vector2f &size, int format) : Texture(size, format) {

}

int PSP2GLTexture::lock(FloatRect *rect, void **pix, int *p) {

    return 0;
}

void PSP2GLTexture::unlock() {

}

void PSP2GLTexture::setFiltering(int filter) {

}

PSP2GLTexture::~PSP2GLTexture() {

}

//
// Created by cpasjuste on 01/12/16.
//

#ifdef __GL2__

#include "cross2d/c2d.h"

#define GL_ABGR_EXT 0x8000

using namespace c2d;

GLTextureBuffer::GLTextureBuffer(const Vector2f &size, Format format) : Texture(size, format) {

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texID);

    if (texID) {

        glBindTexture(GL_TEXTURE_2D, texID);

        switch (format) {
            case Format::RGBA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                break;
#ifndef __GLES2__
            case Format::ARGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, nullptr);
                break;
            case Format::BGRA8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
                break;
            case Format::ABGR8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, nullptr);
                break;
#endif
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, (GLsizei) size.x, (GLsizei) size.y, 0,
                             GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("GLTextureBuffer(%p): couldn't create texture buffer", this);
        } else {
            tex_size = {size.x, size.y};
            Texture::setSize(size.x, size.y);
            Texture::setTexture(this, true);
            available = true;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        printf("GLTextureBuffer(%p): couldn't create texture buffer", this);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //printf("GLTextureBuffer(%p): %ix%i\n", this, (int) size.x, (int) size.y);
}

void GLTextureBuffer::setFilter(Filter f) {

    this->filter = f;

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
}

void GLTextureBuffer::setShader(int shaderIndex) {

    ShaderList *shaderList = c2d_renderer->getShaderList();
    if (shaderIndex >= shaderList->getCount()) {
        shader = shaderList->get(0);
        return;
    }

    shader = shaderList->get(shaderIndex);
}

GLTextureBuffer::~GLTextureBuffer() {

    //printf("~GLTextureBuffer(%p)\n", this);
#ifndef __PSP2__
    if (glIsFramebuffer(fbo) == GL_TRUE) {
#endif
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
#ifndef __PSP2__
    }
#endif

#ifndef __PSP2__
    if (glIsTexture(texID) == GL_TRUE) {
#endif
        //printf("glDeleteTextures(%i)\n", texID);
        glDeleteTextures(1, &texID);
        texID = 0;
#ifndef __PSP2__
    }
#endif
}

#endif // __GL2__

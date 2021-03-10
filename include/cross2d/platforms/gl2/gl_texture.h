//
// Created by cpasjuste on 01/12/16.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "cross2d/skeleton/texture.h"

namespace c2d {

    class GLTexture : public Texture {

    public:

        GLTexture(const std::string &path);

        GLTexture(const unsigned char *buffer, int bufferSize);

        GLTexture(const Vector2f &size = Vector2f(0, 0),
                  Format format = Format::RGBA8);

        ~GLTexture() override;

        int save(const std::string &path) override;

        int lock(FloatRect *rect, void **pixels, int *pitch) override;

        void unlock(void *data = nullptr) override;

        void setFilter(Filter filter) override;

        unsigned int texID = 0;

        unsigned char *pixels = nullptr;

    };
}

#endif //GL_TEXTURE_H

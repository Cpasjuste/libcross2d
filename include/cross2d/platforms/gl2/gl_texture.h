//
// Created by cpasjuste on 01/12/16.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "cross2d/skeleton/texture.h"

namespace c2d {

    class GLTexture : public Texture {

    public:
        explicit GLTexture(const std::string &path);

        explicit GLTexture(const unsigned char *buffer, int bufferSize);

        explicit GLTexture(const Vector2f &size = Vector2f(0, 0),
                           Format format = Format::RGBA8);

        ~GLTexture() override;

        int save(const std::string &path) override;

        int lock(IntRect *rect, uint8_t **pixels, int *pitch) override;

        void unlock() override;

        int resize(const Vector2i &size, bool keepPixels = false) override;

        void setFilter(Filter filter) override;

        unsigned int texID = 0;

        uint8_t *pixels = nullptr;

    private:
        unsigned char *getPixels(int *w, int *h, const unsigned char *buffer = nullptr, int bufferSize = 0);

    };
}

#endif //GL_TEXTURE_H

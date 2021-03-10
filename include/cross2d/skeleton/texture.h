//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "object.h"
#include "shader_list.h"
#include "cross2d/skeleton/sfml/RectangleShape.hpp"

namespace c2d {

    class Texture : public RectangleShape {

    public:

        enum class Format : int {
            RGB565, RGBA8, ARGB8, BGRA8, ABGR8
        };

        enum class Filter : int {
            Point, Linear
        };

        // START - to implement, device specific code
        Texture(const std::string &path);

        Texture(const unsigned char *buffer, int bufferSize);

        Texture(const Vector2f &size = Vector2f(), Format format = Format::RGBA8);

        virtual ~Texture();

        virtual Vector2i getTextureSize() {
            return tex_size;
        }

        virtual int resize(const Vector2f &size, bool copyPixels = false) { return -1; };

        virtual int lock(FloatRect *rect, void **pixels, int *pitch) { return -1; };

        virtual void unlock(void *data = nullptr) {};

        virtual int save(const std::string &path) { return -1; };

        virtual void setFilter(Filter filter) { this->filter = filter; };

        virtual void setShader(int shaderIndex);

        virtual void applyShader() {};
        // END - to implement, device specific code

        std::string path;
        int bpp = 4;
        int pitch = 0;
        Format format = Format::RGBA8;
        Filter filter = Filter::Linear;
        ShaderList::Shader *shader = nullptr;

    protected:
        Vector2i tex_size;
    };
}

#endif //_TEXTURE_H_

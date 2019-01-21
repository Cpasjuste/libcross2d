//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "object.h"
#include "shader_list.h"
#include "cross2d/skeleton/sfml/Shape.hpp"

namespace c2d {

    class Texture : public Shape {

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

        Texture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        virtual ~Texture();

        virtual int resize(const Vector2f &size, bool copyPixels = false) { return -1; };

        virtual int lock(FloatRect *rect, void **pixels, int *pitch) { return -1; };

        virtual void unlock() {};

        virtual int save(const std::string &path) { return -1; };

        virtual void setFilter(Filter filter) { this->filter = filter; };

        virtual void setShader(int shaderIndex) {};

        virtual void applyShader() {};
        // END - to implement, device specific code

        void setSize(const c2d::Vector2f &size);

        void setSize(float width, float height);

        const c2d::Vector2f &getSize() const;

        std::size_t getPointCount() const;

        c2d::Vector2f getPoint(std::size_t index) const;

        void setTextureRect(const IntRect &rect) override;

        std::string path;
        int bpp = 4;
        int pitch = 0;
        Format format = Format::RGBA8;
        Filter filter = Filter::Linear;
        ShaderList::Shader *shader = nullptr;

    private:

        c2d::Vector2f m_size;
    };
}

#endif //_TEXTURE_H_

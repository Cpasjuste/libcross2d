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
            RGB565, RGBA8
        };

        enum class Filter : int {
            Point, Linear
        };

        // START - to implement, device specific code
        explicit Texture(const std::string &path);

        explicit Texture(const unsigned char *buffer, int bufferSize);

        explicit Texture(const Vector2i &size = Vector2i(), Format format = Format::RGBA8);

        virtual ~Texture();

        virtual Vector2i getTextureSize() {
            return m_tex_size;
        }

        virtual int lock(uint8_t **pixels, int *pitch = nullptr, IntRect rect = {0, 0, 0, 0}) { return -1; };

        virtual void unlock() {};

        virtual void setFilter(Filter filter) { m_filter = filter; };

        virtual void setShader(int shaderIndex);

        virtual void setShader(const std::string &shaderName);

        virtual void applyShader() {};
        // END - to implement, device specific code

        virtual int resize(const Vector2i &size, bool keepPixels = false);

        virtual int save(const std::string &path);

        std::string m_path;
        int m_bpp = 4;
        int m_pitch = 0;
        Format m_format = Format::RGBA8;
        Filter m_filter = Filter::Linear;
        ShaderList::Shader *m_shader = nullptr;

    protected:
        Vector2i m_tex_size;
        uint8_t *m_pixels = nullptr;
        IntRect m_unlock_rect = IntRect();
    };
}

#endif //_TEXTURE_H_

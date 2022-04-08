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

        explicit Texture(const std::string &path);

        explicit Texture(const unsigned char *buffer, int bufferSize);

        explicit Texture(const Vector2i &size = Vector2i(), Format format = Format::RGBA8);

        virtual ~Texture();

        virtual int lock(uint8_t **pixels, int *pitch = nullptr, IntRect rect = IntRect());

        virtual void unlock(int rowLength = 0) {};

        virtual int resize(const Vector2i &size, bool keepPixels = false) { return -1; };

        virtual int save(const std::string &path);

        virtual void setFilter(Filter filter) { m_filter = filter; };

        virtual void applyShader() {};

        virtual void setShader(int shaderIndex);

        virtual void setShader(const std::string &shaderName);

        virtual Vector2i getImageSize() {
            return m_image_size;
        }

        virtual Vector2i getTextureSize() {
            return m_tex_size;
        }

        virtual bool isPot() {
            return m_pot;
        }

        std::string m_path;
        int m_bpp = 4;
        int m_pitch = 0;
        Format m_format = Format::RGBA8;
        Filter m_filter = Filter::Linear;
        ShaderList::Shader *m_shader = nullptr;
        uint8_t *m_pixels = nullptr;

    protected:
        void pixelsToPot(int w, int h);

        Vector2i m_tex_size;
        Vector2i m_image_size;
        IntRect m_unlock_rect = IntRect();
#ifdef __3DS__
        bool m_pot = true;
#else
        bool m_pot = false;
#endif
    };
}

#endif //_TEXTURE_H_

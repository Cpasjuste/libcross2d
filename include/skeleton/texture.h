//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "object.h"
#include "shader_list.h"
#include "skeleton/sfml/Transformable.hpp"
#include "skeleton/sfml/VertexArray.hpp"

namespace c2d {

    class Texture : public Transformable {

    public:

        enum class Format : int {
            RGB565, RGBA8, ARGB8, BGRA8, ABGR8
        };

        enum class Filter : int {
            Point, Linear
        };

        // START - to implement, device specific code
        Texture(const char *path);

        Texture(const unsigned char *buffer, int bufferSize);

        Texture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        virtual ~Texture();

        virtual int resize(const Vector2f &size, bool copyPixels = false) { return -1; };

        virtual int lock(FloatRect *rect, void **pixels, int *pitch) { return -1; };

        virtual void unlock() {};

        virtual int save(const char *path) { return -1; };

        virtual void setFilter(Filter filter) { this->filter = filter; };

        virtual void setShader(int shaderIndex) {};

        virtual void applyShader() {};
        // END - to implement, device specific code

        void setTextureRect(const IntRect &rectangle);

        void setOrigin(float x, float y);

        void setOrigin(const Vector2f &origin);

        void setOrigin(const Origin &origin, bool outline = true);

        void setColor(const Color &color);

        const IntRect &getTextureRect() const;

        const Color &getColor() const;

        FloatRect getLocalBounds() const;

        FloatRect getGlobalBounds() const;

        VertexArray *getVertices();

        char path[512];
        int bpp = 4;
        int pitch = 0;
        Format format = Format::RGBA8;
        Filter filter = Filter::Linear;
        ShaderList::Shader *shader = nullptr;

    private:

        virtual void draw(Transform &transform);

        void updatePositions();

        void updateTexCoords();

        VertexArray m_vertices;
        IntRect m_textureRect;
        Origin m_origin = Origin::TopLeft;

    };
}

#endif //_TEXTURE_H_

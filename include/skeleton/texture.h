//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "object.h"
#include "shader_list.h"

#define C2D_TEXTURE_FILTER_POINT    0
#define C2D_TEXTURE_FILTER_LINEAR   1

#define C2D_TEXTURE_FMT_RGB565      0
#define C2D_TEXTURE_FMT_RGBA8       1
#define C2D_TEXTURE_FMT_ARGB8       2

namespace c2d {

    class Texture : public C2DObject, public c2d::Transformable {

    public:

        // START - to implement, device specific code
        Texture(const char *path);

        Texture(const unsigned char *buffer, int bufferSize);

        Texture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        virtual ~Texture();

        virtual int resize(const Vector2f &size, bool copyPixels = false) { return -1; };

        virtual int lock(FloatRect *rect, void **pixels, int *pitch) { return -1; };

        virtual void unlock() {};

        virtual int save(const char *path) { return -1; };

        virtual void setFiltering(int filter) { filtering = filter; };

        virtual void setShader(int shaderIndex) {};

        virtual void applyShader() {};
        // END - to implement, device specific code

        ////////////////// NEW //////////
        void setTextureRect(const IntRect& rectangle);
        void setColor(const Color& color);

        const IntRect& getTextureRect() const;
        const Color& getColor() const;
        FloatRect getLocalBounds() const;
        FloatRect getGlobalBounds() const;
        VertexArray getVertices() const;

        char path[512];
        int format = C2D_TEXTURE_FMT_RGBA8;
        int bpp = 4;
        int pitch = 0;
        int filtering = C2D_TEXTURE_FILTER_LINEAR;
        ShaderList::Shader *shader = NULL;
        //bool available = false;

    private:

        virtual void draw(Transform &transform);

        void updatePositions();
        void updateTexCoords();
        VertexArray m_vertices; ///< Vertices defining the sprite's geometry
        const Texture *m_texture;     ///< Texture of the sprite
        IntRect m_textureRect; ///< Rectangle defining the area of the source texture to display

    };
}

#endif //_TEXTURE_H_

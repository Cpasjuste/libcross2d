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

namespace c2d {

    class Texture : public C2DObject, public sfml::RectangleShape {

    public:

        // START - to implement, device specific code
        Texture(const char *path);

        Texture(const unsigned char *buffer, int bufferSize);

        Texture(const Vector2f &size = Vector2f(0, 0), int format = C2D_TEXTURE_FMT_RGBA8);

        virtual ~Texture();

        virtual int resize(const Vector2f &size, bool copyPixels = true) { return -1; };

        virtual int lock(FloatRect *rect, void **pixels, int *pitch) { return -1; };

        virtual void unlock() {};

        virtual int save(const char *path) { return -1; };

        virtual void setFiltering(int filter) { filtering = filter; };

        virtual void setShader(int shaderIndex) {};

        virtual void applyShader() {};
        // END - to implement, device specific code

        char path[512];

        int format = C2D_TEXTURE_FMT_RGBA8;
        int bpp = 4;
        int pitch = 0;
        int filtering = C2D_TEXTURE_FILTER_LINEAR;
        ShaderList::Shader *shader = NULL;

    private:

        virtual void draw(Transform &transform);

    };
}

#endif //_TEXTURE_H_

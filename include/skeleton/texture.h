//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "widget.h"

#define C2D_TEXTURE_FILTER_POINT    0
#define C2D_TEXTURE_FILTER_LINEAR   1

namespace c2d {

    class Texture : public Widget {

    public:

        // START - to implement, device specific code
        Texture(const char *path);

        Texture(const Vector2f &size = Vector2f(0, 0));

        virtual ~Texture();

        virtual int lock(const FloatRect &rect, void **pixels, int *pitch) { return 0; };

        virtual void unlock() {};

        virtual void setFiltering(int filter) {};
        // END - to implement, device specific code

        char path[512];

    private:

        virtual void draw(const Transform &transform, const Vector2f &scaling);

    };
}

#endif //_TEXTURE_H_

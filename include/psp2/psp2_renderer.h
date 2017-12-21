//
// Created by cpasjuste on 21/11/16.
//

#ifndef _PSP2_RENDERER_H_
#define _PSP2_RENDERER_H_

#include "skeleton/renderer.h"

//#ifdef __PSP2_DEBUG__
#include <psp2/kernel/clib.h>
#define printf sceClibPrintf
//#endif

namespace c2d {

    class PSP2Renderer : public Renderer {

    public:

        PSP2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2Renderer();

        void drawRectangle(
                const Rectangle &rectangle,
                const Transform &transform);

        void drawTexture(const Texture &texture,
                         const Transform &transform);

        void flip();

        void delay(unsigned int ms);

        void setShader(int shader);

    private:

        void startDrawing();
        bool drawing_started = false;
    };
}

#endif //_PSP2_RENDERER_H_

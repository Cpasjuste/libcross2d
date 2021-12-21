//
// Created by cpasjuste on 21/11/16.
//

#ifndef C2D_PS4_RENDERER_H
#define C2D_PS4_RENDERER_H

#include "cross2d/platforms/ps4/ps4_shacc.h"

namespace c2d {

    class PS4Renderer : public GLRenderer {

    public:

        explicit PS4Renderer(const Vector2f &size = Vector2f(0, 0));

        ~PS4Renderer() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

        bool loadModules();

    private:

        bool loadEGL();

        ShaderCompiler *shaderCompiler = nullptr;
        bool modulesLoaded;
        int pigletModuleId = 0;
        ScePglConfig pglConfig;
        EGLDisplay display = EGL_NO_DISPLAY;
        EGLSurface surface = EGL_NO_SURFACE;
        EGLContext context = EGL_NO_CONTEXT;
        SceWindow window = {};
    };
}

#endif // C2D_PS4_RENDERER_H

//
// Created by cpasjuste on 21/11/16.
//

#include <libkernel.h>
#include <libSceSysmodule.h>
#include <libSceSystemService.h>
#include "cross2d/c2d.h"
#include "ps4_renderer.h"

using namespace c2d;

#define MODULE_PIGLET "/data/self/system/common/lib/libScePigletv2VSH.sprx"

PS4Renderer::PS4Renderer(const Vector2f &s) : GLRenderer(s) {

    // load default modules
    if (!loadModules()) {
        return;
    }

    // hide splash screen (is this mandatory ?)
    sceSystemServiceHideSplashScreen();

#ifndef NDEBUG
    // enable debug net on debug builds
    debugNetInit(DEBUG_NET_IP, 18194, 3);
#endif

    // init EGL
    if (!loadEGL()) {
        return;
    }

    // load shader compiler (if possible)
    shaderCompiler = new ShaderCompiler();

    // init GLES2 stuff
    GLRenderer::initGL();

    available = true;

    printf("PS4Renderer(%p): %ix%i\n", this, (int) m_size.x, (int) m_size.y);
}

bool PS4Renderer::loadModules() {
    if (!modulesLoaded) {
        modulesLoaded = true;
        int ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
        if (ret != 0) {
            //printf("PS4Renderer::loadModules: SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE could not be loaded (0x%08x)", ret);
            return false;
        }
        ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_NET);
        if (ret != 0) {
            //printf("PS4Renderer::loadModules: SCE_SYSMODULE_INTERNAL_NET could not be loaded (0x%08x)", ret);
            return false;
        }
        ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_USER_SERVICE);
        if (ret != 0) {
            //printf("PS4Renderer::loadModules: SCE_SYSMODULE_INTERNAL_USER_SERVICE could not be loaded (0x%08x)", ret);
            return false;
        }
        ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PAD);
        if (ret != 0) {
            //printf("PS4Renderer::loadModules: SCE_SYSMODULE_INTERNAL_PAD could not be loaded (0x%08x)", ret);
            return false;
        }
    }

    return true;
}

bool PS4Renderer::loadEGL() {

    EGLConfig config{};
    EGLint num_configs{0};
    int major = -1;
    int minor = -1;

    const EGLint attribs[]{
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 0,
            EGL_STENCIL_SIZE, 0,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES, 0,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE,
    };

    const EGLint ctx_attribs[]{
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE,
    };

    const EGLint window_attribs[]{
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_NONE,
    };

    // load piglet module for gles2
    pigletModuleId = sceKernelLoadStartModule(MODULE_PIGLET, 0, nullptr, 0, nullptr, nullptr);
    if (pigletModuleId < 0) {
        printf("PS4Renderer: could not load piglet module (0x%08x)\n", pigletModuleId);
        return false;
    }

    // create egl surface
    memset(&pglConfig, 0, sizeof(pglConfig));
    pglConfig.size = sizeof(pglConfig);
    pglConfig.flags = SCE_PGL_FLAGS_USE_COMPOSITE_EXT | SCE_PGL_FLAGS_USE_FLEXIBLE_MEMORY | 0x60;
    pglConfig.processOrder = 1;
    pglConfig.systemSharedMemorySize = 0x4000000;
    pglConfig.videoSharedMemorySize = 0x4000000;
    pglConfig.maxMappedFlexibleMemory = 0xFFFFFFFF;
    pglConfig.drawCommandBufferSize = 0x400000;
    pglConfig.lcueResourceBufferSize = 0x2000000;
    pglConfig.dbgPosCmd_0x40 = (uint32_t) getSize().x;
    pglConfig.dbgPosCmd_0x44 = (uint32_t) getSize().y;
    pglConfig.dbgPosCmd_0x48 = 0;
    pglConfig.dbgPosCmd_0x4C = 0;
    pglConfig.unk_0x5C = 2;

    if (!scePigletSetConfigurationVSH(&pglConfig)) {
        printf("PS4Renderer::loadEGL: scePigletSetConfigurationVSH failed\n");
        return false;
    }

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("PS4Renderer::loadEGL: eglGetDisplay failed (0x%08X)\n", eglGetError());
        return false;
    }

    if (!eglInitialize(display, &major, &minor)) {
        printf("PS4Renderer::loadEGL: eglInitialize failed (0x%08X)\n", eglGetError());
        return false;
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API)) {
        printf("PS4Renderer::loadEGL: eglBindAPI failed (0x%08X)\n", eglGetError());
        return false;
    }

    if (!eglSwapInterval(display, 0)) {
        printf("PS4Renderer::loadEGL: eglSwapInterval failed (0x%08X)\n", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &config, 1, &num_configs)) {
        printf("PS4Renderer::loadEGL: eglChooseConfig failed (0x%08X)\n", eglGetError());
        return false;
    }

    window.width = (uint32_t) getSize().x;
    window.height = (uint32_t) getSize().y;
    surface = eglCreateWindowSurface(display, config, &window, window_attribs);
    if (surface == EGL_NO_SURFACE) {
        printf("PS4Renderer::loadEGL: eglCreateWindowSurface failed (0x%08X)\n", eglGetError());
        return false;
    }

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attribs);
    if (context == EGL_NO_CONTEXT) {
        printf("PS4Renderer::loadEGL: eglCreateContext failed (0x%08X)\n", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        printf("PS4Renderer::loadEGL: eglMakeCurrent failed (0x%08X)\n", eglGetError());
        return false;
    }

    return true;
}

void PS4Renderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GLRenderer::flip(draw, inputs);

    // flip
    eglSwapBuffers(display, surface);
}

void PS4Renderer::delay(unsigned int ms) {
    const unsigned int max_delay = 0xffffffffUL / 1000;
    if (ms > max_delay) {
        ms = max_delay;
    }
    sceKernelUsleep(ms * 1000);
}

PS4Renderer::~PS4Renderer() {

    if (shaderCompiler) delete (shaderCompiler);

    if (pigletModuleId > 0) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(display, surface);
        surface = EGL_NO_SURFACE;
        eglDestroyContext(display, context);
        context = EGL_NO_CONTEXT;
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        sceKernelStopUnloadModule(pigletModuleId, 0, nullptr, 0, nullptr, nullptr);
    }

    // TODO: is this needed ?
    //sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PAD);
    //sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_USER_SERVICE);
    //sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_NET);
    //sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
}

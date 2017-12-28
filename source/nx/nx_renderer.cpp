//
// Created by cpasjuste on 21/11/16.
//

#include "../../include/nx/nx_renderer.h"
#include "../../include/nx/nx_texture.h"
#include "../../include/nx/nx_font.h"

//////////
// INIT //
//////////
NXRenderer::NXRenderer() : Renderer() {

    gfxInitDefault();

    // TODO
    this->shaders = new Shaders("");
}
//////////
// INIT //
//////////

//////////
// FONT //
//////////
Font *NXRenderer::LoadFont(const char *path, int size) {

    Font *font = (Font * )
    new NXFont(path, size);
    return font;
}

void NXRenderer::DrawFont(Font *font, int x, int y, const char *fmt, ...) {

    if (((NXFont *) font)->font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    // TODO: draw
}
//////////
// FONT //
//////////

/////////////
// TEXTURE //
/////////////
Texture *NXRenderer::CreateTexture(int w, int h) {
    NXTexture *texture = new NXTexture(w, h);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

Texture *NXRenderer::LoadTexture(const char *file) {
    NXTexture *texture = new NXTexture(file);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

void NXRenderer::DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation) {

    NXTexture *ctr_tex = (NXTexture *) texture;

    // TODO: draw
}

int NXRenderer::LockTexture(Texture *texture, const Rect &rect, void **pixels, int *pitch) {
    *pixels = ((NXTexture *) texture)->pixels;
    *pitch = texture->width * 2;
    return 0;
}

/////////////
// TEXTURE //
/////////////

const Rect NXRenderer::GetWindowSize() {
    Rect rect{0, 0, 1280, 720};
    return rect;
}

void NXRenderer::SetShader(int index) {

}

void NXRenderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    // TODO: draw
}

void NXRenderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    // TODO: draw
}

void NXRenderer::Clip(const Rect &rect) {

}

void NXRenderer::Clear() {
}

void NXRenderer::Flip() {
    gfxSwapBuffers();
    gfxWaitForVsync();
}

void NXRenderer::Delay(unsigned int ms) {
    s64 nano = ms * 1000000;
    svcSleepThread((u64) nano);
}

NXRenderer::~NXRenderer() {
    gfxExit();
}

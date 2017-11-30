//
// Created by cpasjuste on 21/11/16.
//

#include <SDL2/SDL.h>
#include <psp2_renderer.h>
#include "psp2_font.h"
#include "psp2_texture.h"
#include "psp2_shaders.h"

//////////
// INIT //
//////////
PSP2Renderer::PSP2Renderer(int w, int h) : Renderer(w, h) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    vita2d_init();
    vita2d_set_clear_color((unsigned int) RGBA8(color.r, color.g, color.b, color.a));

    this->shaders = (Shaders *) new PSP2Shaders("");
    SetShader(0);
}
//////////
// INIT //
//////////

void PSP2Renderer::SetShader(int index) {

    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;

    vita2d_shader *shader = (vita2d_shader *) shaders->Get()->data;
    if (shader != NULL) {
        vita2d_texture_set_program(shader->vertexProgram, shader->fragmentProgram);
        vita2d_texture_set_wvp(shader->wvpParam);
        vita2d_texture_set_vertexInput(&shader->vertexInput);
        vita2d_texture_set_fragmentInput(&shader->fragmentInput);
    }
}

void PSP2Renderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {
    StartDrawing();
    vita2d_draw_line(x1, y1, x2, y2,
                     RGBA8(c.r, c.g, c.b, c.a));
}

void PSP2Renderer::DrawRect(const Rect &rect, const Color &c, bool fill) {
    StartDrawing();
    if (fill) {
        vita2d_draw_rectangle(rect.x, rect.y, rect.w, rect.h,
                              RGBA8(c.r, c.g, c.b, c.a));
    } else {
        DrawLine(rect.x, rect.y + 1, rect.x + rect.w, rect.y + 1, c);               // top
        DrawLine(rect.x + 1, rect.y, rect.x + 1, rect.y + rect.h, c);               // left
        DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, c);   // bottom
        DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, c);   // right
    }
}

void PSP2Renderer::Clip(const Rect &rect) {

    // vita2d_set_region_clip doesn't work correctly
    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE,
                               rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
    } else {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_NONE, 0, 0, 0, 0);
    }
}

void PSP2Renderer::Clear() {
    StartDrawing();
    vita2d_clear_screen();
}

void PSP2Renderer::Flip() {
    if (drawing_started) {
        vita2d_end_drawing();
        vita2d_wait_rendering_done();
        vita2d_swap_buffers();
        drawing_started = false;
    }
}

void PSP2Renderer::Delay(unsigned int ms) {
    SDL_Delay(ms);
}

PSP2Renderer::~PSP2Renderer() {
    vita2d_wait_rendering_done();
    vita2d_fini();
    delete (shaders);
}

void PSP2Renderer::StartDrawing() {
    if (!drawing_started) {
        vita2d_start_drawing();
        drawing_started = true;
    }
}
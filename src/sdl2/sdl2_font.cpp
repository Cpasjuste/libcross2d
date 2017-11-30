//
// Created by cpasjuste on 01/12/16.
//

#include <C2D.h>

SDL2Font::SDL2Font(Renderer *r, const char *p, int s) : Font(r, p, s) {

    font = FC_CreateFont();
    if (font != nullptr) {
        FC_LoadFont(font, ((SDL2Renderer *) renderer)->sdl_renderer, path, (Uint32) size,
                    FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    }
}

void SDL2Font::Draw(int x, int y, const char *fmt, ...) {

    if (font == nullptr) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    SDL_Color c{color.r, color.g, color.b, color.a};
    FC_Effect effect = FC_MakeEffect(
            FC_AlignEnum::FC_ALIGN_LEFT,
            FC_Scale{scaling, scaling}, c);
    FC_DrawEffect(font, ((SDL2Renderer *) renderer)->sdl_renderer, x, y, effect, msg);
}

int SDL2Font::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) (FC_GetWidth(font, msg) * scaling);
}

int SDL2Font::GetHeight(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) (FC_GetHeight(font, msg) * scaling);
}

SDL2Font::~SDL2Font() {
    if (font != nullptr) {
        FC_FreeFont(font);
        font = nullptr;
    }
}

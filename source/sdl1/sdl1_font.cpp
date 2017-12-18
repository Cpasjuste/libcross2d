//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"

SDL1Font::SDL1Font(Renderer *r, const char *p, int s) : Font(r, p, s) {

    TTF_Init();
    font = TTF_OpenFont(p, s);
    if (!font) {
        printf("SDL1Font: TTF_OpenFont failed\n");
        return;
    }
}

void SDL1Font::Draw(int x, int y, const char *fmt, ...) {

    if (!font) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    SDL_Surface *screen = ((SDL1Renderer *) renderer)->screen;
    SDL_Color col = {color.r, color.g, color.b, 0};
    SDL_Surface *surface = TTF_RenderText_Solid(font, msg, col);
    SDL_Rect dst = {(Sint16) x, (Sint16) y, 0, 0};
    SDL_BlitSurface(surface, NULL, screen, &dst);
}

int SDL1Font::GetWidth(const char *fmt, ...) {

    if (!font) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    int w, h;
    TTF_SizeText(font, msg, &w, &h);

    return w;
}

int SDL1Font::GetHeight(const char *fmt, ...) {

    if (!font) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    int w, h;
    TTF_SizeText(font, msg, &w, &h);

    return h;
}

SDL1Font::~SDL1Font() {

    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

//
// Created by cpasjuste on 01/12/16.
//

#include "../../include/C2D.h"

PSP2Font::PSP2Font(Renderer *r, const char *path, int size) : Font(r, path, size) {
    this->size = size;
    font = vita2d_load_custom_pgf(path);
    if (font == nullptr) {
        printf("error: could not load custom font: %s \n", path);
    }
}

PSP2Font::~PSP2Font() {
    if (font != nullptr) {
        vita2d_free_pgf(font);
        font = nullptr;
    }
}


void PSP2Font::Draw(int x, int y, const char *fmt, ...) {

    if (font == nullptr) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    int height = GetHeight(msg);
    ((PSP2Renderer *) renderer)->StartDrawing();
    vita2d_pgf_draw_text(((PSP2Font *) font)->font, x, y + height,
                         (unsigned int) RGBA8(color.r, color.g,
                                              color.b, color.a),
                         scaling, msg);
}

int PSP2Font::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return vita2d_pgf_text_width(font, scaling, msg);
}

int PSP2Font::GetHeight(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    float ratio = size / 20;
    return (int) ((float) vita2d_pgf_text_height(font, scaling, msg) * ratio - (3 * ratio));
}

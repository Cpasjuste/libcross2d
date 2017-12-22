//
// Created by cpasjuste on 29/11/17.
//

#include "skeleton/font.h"

Font::Font(Renderer *r, const char *p, int s) {

    this->renderer = r;
    this->size = s;
    strncpy(path, p, 512);
}

void Font::Draw(const Rect &dst, const Color &c, const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    Draw(dst, c, false, false, msg);
}

void Font::Draw(const Rect &dst, const Color &c, bool centerX, bool centerY, const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    // cut message "properly" instead of clip
    while (GetWidth(msg) > dst.w) {
        int len = (int) strlen(msg);
        if (len == 0) {
            break;
        }
        msg[strlen(msg) - 1] = 0;
    }

    Rect rect{dst.x, dst.y, dst.w, dst.h};

    if (centerY) {
        rect.y = dst.y + (dst.h / 2) - GetHeight(msg) / 2;
    }

    if (centerX) {
        rect.x = dst.x + (dst.w / 2) - GetWidth(msg) / 2;
    }

    Color old_col = color;
    color = c;
    Draw(rect.x, rect.y, msg);
    color = old_col;
}

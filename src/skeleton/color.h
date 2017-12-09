//
// Created by cpasjuste on 02/12/16.
//

#ifndef _COLOR_H_
#define _COLOR_H_

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() {
        r = 0, g = 0, b = 0, a = 255;
    }

    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
        r = _r, g = _g, b = _b, a = _a;
    }

    uint32_t ToARGB() {
        return (a << 24) + (r << 16) + (g << 8) + (b);
    }

    uint32_t ToRGBA() {
        return (r << 24) + (g << 16) + (b << 8) + (a);
    }
};

extern Color C2D_COL_WHITE;
extern Color C2D_COL_BLACK;
extern Color C2D_COL_RED;
extern Color C2D_COL_ORANGE;
extern Color C2D_COL_YELLOW;
extern Color C2D_COL_GREEN;
extern Color C2D_COL_GRAY;
extern Color C2D_COL_GRAY_LIGHT;
extern Color C2D_COL_BLUE;

#endif //_COLOR_H_

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
};

extern Color WHITE;
extern Color BLACK;
extern Color RED;
extern Color ORANGE;
extern Color YELLOW;
extern Color GREEN;
extern Color GRAY;
extern Color GRAY_LIGHT;

#endif //_COLOR_H_

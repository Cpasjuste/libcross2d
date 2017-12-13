//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_COLOR_H
#define CROSS2D_COLOR_H

namespace C2D {

    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;

        unsigned int ToARGB() {
            return (unsigned int) (a << 24) + (r << 16) + (g << 8) + (b);
        }

        int ToRGBA() {
            return (r << 24) + (g << 16) + (b << 8) + (a);
        }
    };

#define C2D_COL_WHITE       Color{255, 255, 255, 255}
#define C2D_COL_BLACK       Color{0, 0, 0, 255}
#define C2D_COL_RED         Color{255, 0, 0, 255}
#define C2D_COL_ORANGE      Color{255, 165, 0, 255}
#define C2D_COL_YELLOW      Color{255, 255, 0, 255}
#define C2D_COL_GREEN       Color{0, 255, 0, 255}
#define C2D_COL_GRAY        Color{55, 55, 55, 255}
#define C2D_COL_GRAY_LIGHT  Color{40, 40, 40, 255}
#define C2D_COL_BLUE        Color{0, 0, 255, 255}

}

#endif //CROSS2D_COLOR_H

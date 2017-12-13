//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#define C2D_VISIBILITY_VISIBLE  0
#define C2D_VISIBILITY_HIDDEN   1

#include <cstdint>
#include <vector>

namespace C2D {

    struct Rect {
        int x;
        int y;
        int w;
        int h;
    };

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

#define C2D_COL_WHITE Color{0, 0, 0, 255}

    /*
    extern Color C2D_COL_BLACK;
    extern Color C2D_COL_RED;
    extern Color C2D_COL_ORANGE;
    extern Color C2D_COL_YELLOW;
    extern Color C2D_COL_GREEN;
    extern Color C2D_COL_GRAY;
    extern Color C2D_COL_GRAY_LIGHT;
    extern Color C2D_COL_BLUE;
    */

    class Widget {

    public:

        Widget(int posX = 0, int posY = 0, int width = 0, int height = 0,
               float rotation = 0, const Color &color = C2D_COL_WHITE);

        virtual ~Widget();

        virtual void Draw();

        // Childs
        void Add(Widget *widget);

        // position
        Rect GetPosition();

        void SetPosition(const Rect &rect);

        void SetPosition(int posX, int posY,
                         int width = 0, int height = 0,
                         float rotation = 0);

        // rotation
        float GetRotation();

        void SetRotation(float rotation);

        // color
        Color GetColor();

        void SetColor(const Color &color);

        // utility
        void Scale(int size);

    protected:

        Rect rect;
        float rotation;
        Color color;
        int visibility = C2D_VISIBILITY_VISIBLE;
        bool available = true;

        void *renderer;
        Widget *parent;
        std::vector<Widget *> childs;
    };
}

#endif //CROSS2D_DRAWABLE_H

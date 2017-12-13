//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#define C2D_VISIBILITY_VISIBLE  0
#define C2D_VISIBILITY_HIDDEN   1

#include <cstdint>
#include <vector>

#include "color.h"
#include "rect.h"

namespace C2D {

    class Widget {

    public:

        Widget(int posX = 0, int posY = 0, int width = 0, int height = 0,
               float rotation = 0, const Color &color = C2D_COL_BLACK);

        virtual ~Widget();

        virtual void Draw();

        // Childs
        void Add(Widget *widget);

        // position
        Rect GetRect();

        void SetRect(const Rect &rect);

        void SetRect(int posX, int posY, int width = 0, int height = 0);

        // rotation
        float GetRotation();

        void SetRotation(float rotation);

        // color
        Color GetColor();

        void SetColor(const Color &color);

        // scaling
        float GetScaling();

        void SetScaling(float scaling);

        void Scale(int pixels);

    protected:

        Rect rect;
        float rotation;
        float scaling;
        Color color;
        int visibility = C2D_VISIBILITY_VISIBLE;
        bool available = true;

        Widget *renderer;
        Widget *parent;
        std::vector<Widget *> childs;
    };
}

#endif //CROSS2D_DRAWABLE_H

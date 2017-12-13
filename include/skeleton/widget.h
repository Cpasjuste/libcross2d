//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#define C2D_VISIBILITY_VISIBLE  0
#define C2D_VISIBILITY_HIDDEN   1

#define C2D_CENTER_TOP_LEFT     0
#define C2D_CENTER_MIDDLE       1

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

        Rect GetRectAbs();

        void SetRect(const Rect &rect);

        void SetRect(int posX, int posY, int width = 0, int height = 0);

        void Move(int deltaX, int deltaY);

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

        // center
        int GetCenter();

        void SetCenter(int center);

        // visibility
        int GetVisibility();

        void SetVisibility(int visibility);

    protected:

        void Update();

        Rect rect;
        Rect rect_abs;
        float rotation = 0;
        float scaleX = 1;
        float scaleY = 1;
        Color color = C2D_COL_BLACK;
        int center = C2D_CENTER_TOP_LEFT;
        int visibility = C2D_VISIBILITY_VISIBLE;
        bool available = true;

        Widget *renderer;
        Widget *parent;
        std::vector<Widget *> childs;
    };
}

#endif //CROSS2D_DRAWABLE_H

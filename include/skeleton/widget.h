//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#define C2D_VISIBILITY_VISIBLE  0
#define C2D_VISIBILITY_HIDDEN   1

#define C2D_ORIGIN_TOP_LEFT     0
#define C2D_ORIGIN_TOP_RIGHT    1
#define C2D_ORIGIN_CENTER       2
#define C2D_ORIGIN_BOTTOM_LEFT  3
#define C2D_ORIGIN_BOTTOM_RIGHT 4


#include <cstdint>
#include <vector>

#include "color.h"
#include "vector.h"

namespace C2D {

    class Widget {

    public:

        Widget(int x = 0, int y = 0, int w = 0, int h = 0,
               const Color &color = C2D_COL_BLACK,
               int center = C2D_ORIGIN_TOP_LEFT,
               float rot = 0);

        virtual ~Widget();

        virtual void Draw();

        // Childs
        void Add(Widget *widget);

        // position
        Vec4 GetRect();

        Vec4 GetRectWorld();

        Vec4 GetRectLocal();

        void SetRect(const Vec4 &rect);

        void SetRect(int x, int y, int w = 0, int h = 0);

        void Move(int x, int y);

        // rotation
        float GetRotation();

        void SetRotation(float rotation);

        // color
        Color GetColor();

        void SetColor(const Color &color);

        // scaling
        Vec2 GetScaling();

        void SetScaling(const Vec2 &scaling);

        void Scale(int pixels);

        // origin
        Vec2 GetOrigin();

        void SetOrigin(int pivotMode);

        void SetOrigin(const Vec2 &origin);

        // visibility
        int GetVisibility();

        void SetVisibility(int visibility);

    protected:

        virtual void Update();

        Vec4 rect;
        Vec4 rect_world;
        Vec4 rect_local;
        float rotation = 0;
        Vec2 scale = {1, 1};
        Vec2 origin = {0, 0};
        Color color = C2D_COL_BLACK;
        int visibility = C2D_VISIBILITY_VISIBLE;
        bool available = true;

        Widget *renderer;
        Widget *parent;
        std::vector<Widget *> childs;
    };
}

#endif //CROSS2D_DRAWABLE_H

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

    class Renderer;

    class Widget {

    public:

        Widget(int x = 0, int y = 0, int w = 0, int h = 0,
               const Color &color = C2D_COL_BLACK,
               int center = C2D_ORIGIN_TOP_LEFT,
               float rot = 0);

        Widget(const Vec4 &rect = {0, 0, 0, 0},
               const Color &color = C2D_COL_BLACK,
               int center = C2D_ORIGIN_TOP_LEFT,
               float rot = 0);

        virtual ~Widget();

        // Childs
        void add(Widget *widget);

        // position
        Vec4 getRect();

        Vec4 getRectWorld();

        void setRect(const Vec4 &rect);

        void setRect(int x, int y, int w = 0, int h = 0);

        void move(int x, int y);

        // rotation
        float getRotation();

        void setRotation(float rotation);

        void rotate(float rotation);

        // color
        Color getColor();

        void setColor(const Color &color);

        // scaling
        Vec2 getScaling();

        void setScaling(const Vec2 &scaling);

        void scale(int pixels);

        // origin
        Vec2 getOrigin();

        void setOrigin(int pivotMode);

        void setOrigin(const Vec2 &origin);

        // visibility
        int getVisibility();

        void setVisibility(int visibility);

    protected:

        virtual void draw(Renderer *renderer);

        void Update();

        Vec4 rect_origin;
        Vec4 rect_origin_translated;
        Vec4 rect_world;

        float rotation = 0;
        float rotation_world = 0;

        Vec2 scaling = {1, 1};
        Vec2 scaling_world = {1, 1};

        Vec2 origin = {0, 0};
        Color color = C2D_COL_BLACK;
        int visibility = C2D_VISIBILITY_VISIBLE;
        bool available = true;

        //Widget *renderer;
        Widget *parent;
        std::vector<Widget *> childs;

    private:
        bool update_matrix = false;

    };
}

#endif //CROSS2D_DRAWABLE_H

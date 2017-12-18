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

#include "skeleton/c2d_sfml/RectangleShape.hpp"

namespace c2d {

    class Renderer;

    class Widget : public RectangleShape {

    public:

        Widget(const Vector2f &size = Vector2f(0, 0));

        virtual ~Widget();

        virtual void draw(Renderer *renderer, const Transform &transform);

        // Childs
        void add(Widget *widget);

        // visibility
        int getVisibility();

        void setVisibility(int visibility);

    protected:

        int visibility = C2D_VISIBILITY_VISIBLE;

        Widget *parent;
        std::vector<Widget *> childs;

    };
}

#endif //CROSS2D_DRAWABLE_H

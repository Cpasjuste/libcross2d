//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#define C2D_VISIBILITY_VISIBLE  0
#define C2D_VISIBILITY_HIDDEN   1

#include <cstdint>
#include <vector>

#include "skeleton/c2d_sfml/RectangleShape.hpp"

namespace c2d {

    class Renderer;

    class Widget : public RectangleShape {

    public:

        Widget(const FloatRect &rect);

        Widget(const Vector2f &size = Vector2f(0, 0));

        virtual ~Widget();

        virtual void draw(Transform &transform);

        // Childs
        void add(Widget *widget);

        // visibility
        int getVisibility();

        void setVisibility(int visibility);

        bool available = false;

    protected:

        int visibility = C2D_VISIBILITY_VISIBLE;

        Widget *parent = NULL;
        std::vector<Widget *> childs;
    };
}

#endif //CROSS2D_DRAWABLE_H

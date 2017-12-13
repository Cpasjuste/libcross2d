//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_RECTANGLE_H
#define CROSS2D_RECTANGLE_H

#include "widget.h"

namespace C2D {

    class Rectangle : public Widget {

    public:

        Rectangle(int posX = 0, int posY = 0, int width = 0, int height = 0,
                  float rotation = 0, const Color &color = C2D_COL_BLACK);

        virtual ~Rectangle();

        virtual void Draw();

    };
}

#endif //CROSS2D_RECTANGLE_H

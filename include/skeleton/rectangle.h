//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_RECTANGLE_H
#define CROSS2D_RECTANGLE_H

#include "widget.h"

namespace C2D {

    class Rectangle : public Widget {

    public:

        Rectangle(int x = 0, int y = 0, int w = 0, int h = 0,
                  const Color &color = C2D_COL_BLACK,
                  int center = C2D_ORIGIN_TOP_LEFT,
                  float rot = 0,
                  bool fill = true);

        virtual ~Rectangle();

        virtual void Draw();

        bool fill = true;

    };
}

#endif //CROSS2D_RECTANGLE_H

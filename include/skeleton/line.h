//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_LINE_H
#define CROSS2D_LINE_H

#include "widget.h"

namespace C2D {

    class Line : public Widget {

    public:

        Line(int x1 = 0, int x2 = 0, int y1 = 0, int y2 = 0,
             float rotation = 0, const Color &color = C2D_COL_BLACK);

        virtual ~Line();

        virtual void Draw();

    };
}

#endif //CROSS2D_LINE_H

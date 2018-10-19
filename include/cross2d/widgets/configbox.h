//
// Created by cpasjuste on 19/10/18.
//

#ifndef C2D_OPTIONBOX_H
#define C2D_OPTIONBOX_H

#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "listbox.h"

namespace c2d {

    class ConfigBox : public RectangleShape {

    public:

        explicit ConfigBox(const c2d::Vector2f &size = c2d::Vector2f(0, 0));

        explicit ConfigBox(const c2d::FloatRect &rect = c2d::FloatRect(0, 0, 0, 0));


    private:

        ListBox *listBox;

    };
}

#endif //C2D_OPTIONBOX_H

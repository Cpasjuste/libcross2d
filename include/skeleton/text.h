//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_TEXT_H
#define CROSS2D_TEXT_H

#include "skeleton/c2d_sfml/Text.hpp"

namespace c2d {

    class Text : public Widget, public sfml::Text {

    public:

        Text();

        Text(const c2d::String &string, const c2d::Font &font, unsigned int characterSize = 30);

        virtual ~Text();

    private:

        virtual void draw(Transform &transform);
    };
}

#endif //CROSS2D_TEXT_H

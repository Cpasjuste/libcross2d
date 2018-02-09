//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_TEXT_H
#define CROSS2D_TEXT_H

#include "skeleton/c2d_sfml/Text.hpp"

#define C2D_DEFAULT_CHAR_SIZE 30

namespace c2d {

    class Text : public Widget, public sfml::Text {

    public:

        Text();

        Text(const std::string &string, const Font &font,
             unsigned int characterSize = C2D_DEFAULT_CHAR_SIZE);

        virtual ~Text();

    private:

        virtual void draw(Transform &transform);
    };
}

#endif //CROSS2D_TEXT_H

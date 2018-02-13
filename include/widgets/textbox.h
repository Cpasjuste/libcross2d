//
// Created by cpasjuste on 29/12/17.
//

#ifndef CROSS2D_TEXTBOX_H
#define CROSS2D_TEXTBOX_H

#include "skeleton/rectangle.h"
#include "skeleton/text.h"

namespace c2d {

    class TextBox : public Rectangle {

    public:

        TextBox(const std::string &string, const Font &font,
                const FloatRect &rect = FloatRect(),
                const Color &fillColor = Color::GrayLight,
                const Color &textColor = Color::White);

        ~TextBox();

        virtual Text *getText();

    private:

        virtual void draw(Transform &transform);

        Text *text;

    };
}

#endif //CROSS2D_TEXTBOX_H

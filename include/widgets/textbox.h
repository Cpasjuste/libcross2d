//
// Created by cpasjuste on 29/12/17.
//

#ifndef CROSS2D_TEXTBOX_H
#define CROSS2D_TEXTBOX_H

#include "skeleton/sfml/RectangleShape.hpp"
#include "skeleton/sfml/Text.hpp"

namespace c2d {

    class TextBox : public RectangleShape {

    public:

        TextBox(const std::string &string, const Font &font,
                const FloatRect &rect = FloatRect(),
                const Color &fillColor = Color::GrayLight,
                const Color &textColor = Color::White);

        ~TextBox() override;

        virtual Text *getText();

    private:

        void draw(Transform &transform) override;

        Text *text;

    };
}

#endif //CROSS2D_TEXTBOX_H

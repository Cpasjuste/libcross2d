//
// Created by cpasjuste on 29/12/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

TextBox::TextBox(const std::string &string, const Font &font,
                 const FloatRect &rect,
                 const Color &fillColor,
                 const Color &textColor) : RectangleShape(rect) {

    setFillColor(fillColor);

    text = new Text(string, font);
    text->setFillColor(textColor);

    if (rect.width <= 0 || rect.height <= 0) {
        setSize(text->getLocalBounds().width + 16, text->getLocalBounds().height + 16);
    }

    text->setOrigin(Origin::Center);
    text->setPosition(getLocalBounds().width / 2, getLocalBounds().height / 2);

    add(text);
}

void TextBox::draw(Transform &transform) {

    // update text position
    //text->setOrigin(text->getLocalBounds().width / 2, text->getLocalBounds().height / 2);
    text->setPosition(getLocalBounds().width / 2, getLocalBounds().height / 2);

    Shape::draw(transform);
}

Text *TextBox::getText() {
    return text;
}

TextBox::~TextBox() {

    if (text) {
        delete (text);
    }
}

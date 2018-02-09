//
// Created by cpasjuste on 29/12/17.
//

#include "c2d.h"

using namespace c2d;

TextBox::TextBox(const std::string &string, const Font &font,
                 const FloatRect &rect,
                 const Color &fillColor,
                 const Color &textColor) : Rectangle(rect) {

    setFillColor(fillColor);

    text = new Text(string, font);
    text->setFillColor(textColor);

    if (!rect.width || !rect.height) {
        setSize(text->getLocalBounds().width + 16, text->getLocalBounds().height + 16);
    }

    text->setOriginCenter();
    text->setPosition(getLocalBounds().width / 2, getLocalBounds().height / 2);

    add(text);
}

void TextBox::draw(Transform &transform) {

    // update text position
    //text->setOrigin(text->getLocalBounds().width / 2, text->getLocalBounds().height / 2);
    text->setPosition(getLocalBounds().width / 2, getLocalBounds().height / 2);

    // draw rect from renderer
    c2d_renderer->drawRectangle(*this, transform);

    // call base class (draw childs)
    Widget::draw(transform);
}

Text *TextBox::getText() {
    return text;
}

TextBox::~TextBox() {

    if (text) {
        delete (text);
    }
}

//
// Created by cpasjuste on 05/02/18.
//

#include "cross2d/c2d.h"

using namespace c2d;

Button::Button(const c2d::FloatRect &rect, c2d::Font &font,
               int fontSize, const std::string &text)
        : RectangleShape(rect) {

    this->text = new Text(text, font, (unsigned int) fontSize);
    this->text->setPosition(getSize().x / 2, getSize().y / 2);
    this->text->setOutlineColor(Color::Black);
    this->text->setOutlineThickness(1);
    add(this->text);

    setText(text);
}

void Button::setText(const std::string &text) {

    this->text->setScale(1, 1);
    this->text->setString(text);
    this->text->setOrigin(Origin::Center);
    float width = this->text->getGlobalBounds().width;
    if (width > getSize().x - 8) {
        float scaling = (getSize().x - 16) / width;
        this->text->setScale(scaling, scaling);
    }
}

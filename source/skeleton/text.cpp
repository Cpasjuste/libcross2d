//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Text::Text() {

    printf("Text(%p): %ix%i\n", this, (int) getLocalBounds().width, (int) getLocalBounds().height);

    thisTransform = this;
}

Text::Text(const String &string, const Font &font, unsigned int characterSize)
        : SFMLText(string, font, characterSize) {

    printf("Text(%p): %ix%i\n", this, (int) getLocalBounds().width, (int) getLocalBounds().height);

    thisTransform = this;
}

void Text::draw(Transform &transform) {

    printf("Text(%p): draw\n", this);

    // draw rect from renderer
    SFMLText::draw(c2d_renderer, transform);

    // call base class (draw childs)
    Widget::draw(transform);
}

Text::~Text() {

    printf("~Text(%p)\n", this);
}

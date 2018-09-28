//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Text::Text(const std::string &string, const Font &font, unsigned int characterSize)
        : sfml::Text(string, font, characterSize) {

    //printf("Text(%p): %s (%ix%i)\n", this, string.c_str(),
    //        (int) getLocalBounds().width, (int) getLocalBounds().height);

    type = TText;
}

void Text::draw(Transform &transform) {

    // draw text from renderer
    c2d_renderer->drawText(*this, transform);

    // call base class (draw childs)
    C2DObject::draw(transform);
}

Text::~Text() {

    //printf("~Text(%p): %s\n", this, getString().c_str());
}

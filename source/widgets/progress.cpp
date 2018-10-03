//
// Created by cpasjuste on 03/10/18.
//

#include "c2d.h"

using namespace c2d;

c2d::Progress::Progress(const c2d::FloatRect &rect) : Rectangle(rect) {

    progress_fg = new Rectangle(
            FloatRect(1, 1, 2, getSize().y - 2));
    progress_fg->setFillColor(Color::Orange);
    add(progress_fg);
}

void Progress::setBgColor(const c2d::Color &color) {
    setFillColor(color);
}

void Progress::setFgColor(const c2d::Color &color) {
    progress_fg->setFillColor(color);
}

void Progress::setProgress(float progress) {

    float width = progress * (getSize().x - 2);
    progress_fg->setSize(
            std::min(width, getSize().x - 2),
            progress_fg->getSize().y);
}

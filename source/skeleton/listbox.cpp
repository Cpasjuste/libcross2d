//
// Created by cpasjuste on 05/01/18.
//

#include "skeleton/listbox.h"

using namespace c2d;

#define MIN_SIZE_Y  200

ListBox::ListBox(const Font &font, int fontSize, const FloatRect &rect,
                 const std::vector<Io::File *> &fileList) : Rectangle(rect) {

    files = fileList;

    // set default bg colors
    setFillColor(Color::GrayLight);
    setOutlineColor(Color::Orange);
    setOutlineThickness(2);

    line_height = fontSize + 4;
    max_lines = (int) getSize().y / line_height;

    // add selection rectangle (highlight)
    rectangle = new Rectangle(Vector2f(getSize().x - 4, line_height));
    rectangle->setOutlineThickness(1);
    rectangle->setOrigin(0, line_height / 2);
    add(rectangle);

    // add lines of text
    for (int i = 0; i < max_lines; i++) {
        lines.push_back(new Text());
        lines[i]->setFont(font);
        lines[i]->setCharacterSize((unsigned int) fontSize);
        lines[i]->setOutlineThickness(getSize().y < MIN_SIZE_Y ? 1 : 2);
        lines[i]->setOutlineColor(Color::Black);
        lines[i]->setPosition(8, 12 + line_height * i);
        lines[i]->setSizeMax(Vector2f(getSize().x, 0));
        add(lines[i]);
    }

    // update texts
    setSelection(0);
};

ListBox::~ListBox() {
    // no need to delete lines pointer,
    // will be delete by parent (widget)
    lines.clear();
}

void ListBox::setSelection(int idx) {

    if (files.size() < 1) {
        rectangle->setVisibility(C2D_VISIBILITY_HIDDEN);
        return;
    } else {
        rectangle->setVisibility(C2D_VISIBILITY_VISIBLE);
    }

    index = idx;
    int page = index / max_lines;
    int index_start = page * max_lines;

    for (int i = 0; i < max_lines; i++) {

        if (index_start + i >= (int) files.size()) {
            lines[i]->setVisibility(C2D_VISIBILITY_HIDDEN);
        } else {

            // set file
            Io::File *file = files.at((unsigned long) (index_start + i));
            lines[i]->setVisibility(C2D_VISIBILITY_VISIBLE);
            lines[i]->setString(file->name.c_str());

            // set text color based on file color
            lines[i]->setFillColor(file->color);

            // set highlight position and color
            if (index_start + i == index) {
                rectangle->setPosition(2, lines[i]->getGlobalBounds().top - 1);
                Color color = file->color;
                rectangle->setOutlineColor(color);
#ifdef __TINYGL__
                color.r *= 0.25f;
                color.g *= 0.25f;
                color.b *= 0.25f;
#else
                color.a = 100;
#endif
                rectangle->setFillColor(color);
            }
        }
    }
}

std::vector<c2d::Text *> ListBox::getLines() {
    return lines;
}

std::vector<c2d::Io::File *> ListBox::getFiles() {
    return files;
}

Io::File *ListBox::getSelection() {
    return files[index];
}

int ListBox::getIndex() {
    return index;
}

void ListBox::setHighLight(bool enable) {
    rectangle->setVisibility(enable ? C2D_VISIBILITY_VISIBLE
                                    : C2D_VISIBILITY_HIDDEN);
}

int ListBox::getMaxLines() {
    return max_lines;
}

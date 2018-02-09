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

    line_height = (int) font.getLineSpacing((unsigned int) fontSize) + 8;
    max_lines = (int) getSize().y / line_height;

    // add selection rectangle (highlight)
    highlight = new Rectangle(Vector2f(getSize().x - 4, line_height));
    highlight->setOutlineThickness(1);
    add(highlight);

    // add lines of text
    for (int i = 0; i < max_lines; i++) {
        Text *text = new Text("LINE", font, (unsigned int) fontSize);
        text->setOutlineThickness(getSize().y < MIN_SIZE_Y ? 1 : 2);
        text->setOutlineColor(Color::Black);
        text->setPosition(8, 8 + line_height * i);
        text->setSizeMax(Vector2f(getSize().x, 0));
        add(text);
        lines.push_back(text);
    }

    // update texts
    setSelection(0);
};

ListBox::~ListBox() {
    // no need to delete lines widgets (ptr),
    // will be delete by parent (widget)
    lines.clear();
}

void ListBox::setSelection(int idx) {

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
            lines[i]->setString(file->name);
            // set text color based on file color
            lines[i]->setFillColor(file->color);

            // set highlight position and color
            if (index_start + i == index) {
                highlight->setPosition(2, lines[i]->getPosition().y - 6);
                Color color = file->color;
                highlight->setOutlineColor(color);
#ifdef __TINYGL__
                color.r *= 0.25f;
                color.g *= 0.25f;
                color.b *= 0.25f;
#else
                color.a = 100;
#endif
                highlight->setFillColor(color);
            }
        }
    }

    if (files.size() < 1) {
        highlight->setVisibility(C2D_VISIBILITY_HIDDEN);
    } else {
        highlight->setVisibility(C2D_VISIBILITY_VISIBLE);
    }
}

std::vector<c2d::Text *> ListBox::getLines() {
    return lines;
}

std::vector<c2d::Io::File *> ListBox::getFiles() {
    return files;
}

Io::File *ListBox::getSelection() {
    return (int) files.size() <= index ? NULL : files[index];

}

int ListBox::getIndex() {
    return index;
}

void ListBox::setHighLight(bool enable) {
    highlight->setVisibility(enable ? C2D_VISIBILITY_VISIBLE
                                    : C2D_VISIBILITY_HIDDEN);
}

int ListBox::getMaxLines() {
    return max_lines;
}

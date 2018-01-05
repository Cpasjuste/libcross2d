//
// Created by cpasjuste on 05/01/18.
//

#include "skeleton/listbox.h"

using namespace c2d;

#define MIN_SIZE_Y  200

ListBox::ListBox(const Font &font,
                 const FloatRect &rect,
                 const std::vector<Io::File *> &fileList) : Rectangle(rect) {

    files = fileList;

    // set bg colors
    setFillColor(Color::GrayLight);
    setOutlineColor(Color::Orange);
    setOutlineThickness(2);

    // calculate line height and maximum lines per page, adjust font size if needed
    float font_scaling = 1;
    line_height = (int) font.getLineSpacing(C2D_DEFAULT_CHAR_SIZE) + 12;
    max_lines = (int) getSize().y / line_height;
    if (max_lines < 15) {
        // scale text to see enough lines on small screens
        max_lines = getSize().y < MIN_SIZE_Y ? 10 : 15;
        font_scaling = (getSize().y / (float) max_lines) / line_height;
        font_size *= font_scaling;
        line_height *= font_scaling;
    }

    // add selection rectangle (highlight)
    rectangle = new Rectangle(Vector2f(getSize().x - 4, line_height));
    rectangle->setFillColor(Color(32, 32, 32));
    rectangle->setOutlineThickness(1);
    rectangle->setOrigin(0, line_height / 2);
    add(rectangle);

    // add lines of text
    for (int i = 0; i < max_lines; i++) {
        lines.push_back(new Text());
        lines[i]->setFont(font);
        lines[i]->setCharacterSize((unsigned int) font_size);
        lines[i]->setOutlineThickness(getSize().y < MIN_SIZE_Y ? 1 : 2);
        lines[i]->setOutlineColor(Color::Black);
        lines[i]->setPosition(8 * font_scaling, (12 * font_scaling) + line_height * i);
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

void ListBox::setSelection(int index) {

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
                rectangle->setPosition(2, lines[i]->getGlobalBounds().top);
                Color color = file->color;
                rectangle->setOutlineColor(color);
                color.a = 100;
                rectangle->setFillColor(color);
            }
        }
    }
}

int ListBox::getMaxLines() {
    return max_lines;
}

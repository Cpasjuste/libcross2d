//
// Created by cpasjuste on 05/01/18.
//

#include "widgets/listbox.h"

using namespace c2d;

#define MIN_SIZE_Y  200

ListBox::ListBox(const Font &font, int fontSize, const FloatRect &rect,
                 const std::vector<Io::File *> &fileList) : Rectangle(rect) {

    printf("ListBox\n");

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
        text->setPosition(8, 6 + line_height * i);
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
    //lines.clear();
    printf("~ListBox\n");
}

void ListBox::setSelection(int idx) {

    index = idx;
    int page = index / max_lines;
    int index_start = page * max_lines;

    for (int i = 0; i < max_lines; i++) {

        if (index_start + i >= (int) files.size()) {
            lines[i]->setVisibility(Hidden);
        } else {
            // set file
            Io::File *file = files.at((unsigned long) (index_start + i));
            lines[i]->setVisibility(Visible);
            lines[i]->setString(file->name);
            // set text color based on file color
            lines[i]->setFillColor(file->color);

            // set highlight position and color
            if (index_start + i == index) {
                highlight->setPosition(2, lines[i]->getPosition().y - 4);
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
        highlight->setVisibility(Hidden);
    } else {
        highlight->setVisibility(Visible);
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
    highlight->setVisibility(enable ? Visible
                                    : Hidden);
}

int ListBox::getMaxLines() {
    return max_lines;
}

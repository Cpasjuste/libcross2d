//
// Created by cpasjuste on 05/01/18.
//

#include <widgets/listbox.h>

#include "widgets/listbox.h"

using namespace c2d;

ListBoxLine::ListBoxLine(
        const FloatRect &rect, const std::string &str,
        const Font &font, unsigned int fontSize, Texture *i, bool ui) : RectangleShape(rect) {

    printf("ListBoxLine(%p)\n", this);


    icon = i;
    use_icons = ui;
    text = new Text(str, font, fontSize);
    text->setOutlineThickness(2);
    text->setOutlineColor(Color::Black);
    text->setOrigin(Origin::Left);

    setFillColor(Color::Transparent);

    if (use_icons) {
        // add iconRect
        iconRect = new C2DRectangle(Vector2f(32, 32));
        iconRect->setOrigin(Origin::Center);
        iconRect->setPosition(Vector2f(1, getSize().y / 2));
        iconRect->setFillColor(Color::Transparent);
        iconRect->setOutlineThickness(1);
        add(iconRect);
        // icon added in ListBox::setSelection (setIcon)
        // set text
        text->setPosition(iconRect->getSize().x + 8, getSize().y / 2);
        text->setSizeMax(Vector2f(getSize().x - 8 - iconRect->getSize().x, 0));
    } else {
        text->setPosition(8, getSize().y / 2);
        text->setSizeMax(Vector2f(getSize().x - 8, 0));
    }

    add(text);
}

void ListBoxLine::setString(const std::string &string) {

    text->setString(string);
}

void ListBoxLine::setColor(const Color &color) {

    text->setFillColor(color);
    if (iconRect) {
        iconRect->setOutlineColor(color);
    }

    // TODO
    //if (icon) {
    //    icon->setOutlineColor(color);
    //}
}

void ListBoxLine::setIcon(Texture *i) {

    if (use_icons) {
        if (icon) {
            remove(icon);
        }
        icon = i;
        if (icon) {
            if (icon->available) {
                // TODO
                //icon->setOutlineThickness(1);
                icon->setOrigin(iconRect->getOrigin());
                icon->setPosition(iconRect->getPosition());
                if (icon->getLocalBounds().width != 32 || icon->getLocalBounds().height != 32) {
                    printf("ListBoxLine: icon size != 32, scaling icon...\n");
                    float scaling = std::min(
                            iconRect->getSize().y / icon->getLocalBounds().width,
                            iconRect->getSize().y / icon->getLocalBounds().height);
                    icon->setScale(scaling, scaling);
                }
            }
            add(icon);
            icon->setVisibility(icon->available ? Visibility::Visible : Visibility::Hidden);
            iconRect->setVisibility(icon->available ? Visibility::Hidden : Visibility::Visible);
        } else {
            iconRect->setVisibility(Visibility::Visible);
        }
    }
}

Text *ListBoxLine::getText() {
    return text;
}

ListBoxLine::~ListBoxLine() {
    printf("~ListBoxLine(%p)\n", this);
}

ListBox::ListBox(const Font &font, int fontSize, const FloatRect &rect,
                 const std::vector<Io::File> &fileList, bool useIcons)
        : RectangleShape(rect) {

    printf("ListBox(%p)\n", this);

    files = fileList;
    use_icons = useIcons;

    // set default bg colors
    setFillColor(Color::GrayLight);
    setOutlineColor(Color::Orange);
    setOutlineThickness(2);

    // calculate number of lines shown
    if (use_icons) {
        line_height = 34; // 32px + 2px margin
    } else {
        line_height = (int) font.getLineSpacing((unsigned int) fontSize) + 8;
    }
    max_lines = (int) (getSize().y / line_height);
    if ((max_lines * line_height) < getSize().y) {
        line_height = getSize().y / (float) max_lines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape(Vector2f(getSize().x - 2, line_height - 2));
    highlight->setOutlineThickness(1);
    add(highlight);

    // add lines of text
    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {
        FloatRect r = {1, (line_height * i) + 1, getSize().x - 2, line_height - 2};
        Texture *icon = nullptr;
        if (use_icons) {
            icon = files.size() > i ? files[i].icon : nullptr;
        }
        ListBoxLine *line = new ListBoxLine(r, "BLpT92,(", font, (unsigned int) fontSize, icon, use_icons);
        lines.push_back(line);
        add(line);
    }

    // update texts
    setSelection(0);
};

ListBox::~ListBox() {
    // no need to delete lines widgets (ptr),
    // will be delete by parent (widget)
    //lines.clear();
    printf("~ListBox(%p)\n", this);
    files.clear();
}

void ListBox::setSelection(int idx) {

    index = idx;
    int page = index / max_lines;
    unsigned int index_start = (unsigned int) page * max_lines;

    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {

        if (index_start + i >= files.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
        } else {
            // set file
            Io::File file = files[index_start + i];
            lines[i]->setVisibility(Visibility::Visible);
            lines[i]->setString(file.name);
            // set text color based on file color
            lines[i]->setIcon(file.icon);
            lines[i]->setColor(file.color);
            // set highlight position and color
            if (index_start + i == (unsigned int) index) {
                highlight->setPosition(lines[i]->getPosition());
                Color color = highlight_use_files_color ?
                              file.color : highlight->getFillColor();
                color.a = highlight_use_files_color ? (uint8_t) 80 : color.a;
                highlight->setFillColor(color);
                color.a = 255;
                highlight->setOutlineColor(color);
            }
        }
    }

    if (files.empty()) {
        highlight->setVisibility(Visibility::Hidden, false);
    } else {
        highlight->setVisibility(Visibility::Visible);
    }
}

void ListBox::setFiles(const std::vector<Io::File> &fileList) {

    files = fileList;
    setSelection(0);
}

std::vector<c2d::Io::File> ListBox::getFiles() {
    return files;
}

Io::File ListBox::getSelection() {
    Io::File file;
    return (files.empty() ||
            (int) files.size() <= index) ? file : files[index];
}

std::vector<ListBoxLine *> ListBox::getLines() {
    return lines;
}

void ListBox::setTextOutlineColor(const Color &color) {
    for (auto &line : lines) {
        line->getText()->setOutlineColor(color);
    }
}

void ListBox::setTextOutlineThickness(float thickness) {
    for (auto &line : lines) {
        line->getText()->setOutlineThickness(thickness);
    }
}

void ListBox::setHighlight(bool enable) {
    highlight->setVisibility(enable ? Visibility::Visible
                                    : Visibility::Hidden);
}

void ListBox::setHighlightTween(Tween *tween) {
    highlight->add(tween);
}

void ListBox::setHighlightColor(const Color &color) {
    highlight->setFillColor(color);
}

void ListBox::setHighlightAlpha(u_int8_t alpha) {
    Color c = highlight->getFillColor();
    highlight->setFillColor({c.r, c.g, c.b, alpha});
}

void ListBox::setHighlightThickness(float thickness) {
    highlight->setOutlineThickness(thickness);
}

void ListBox::setHighlightUseFileColor(bool enable) {
    highlight_use_files_color = enable;
}

int ListBox::getIndex() {
    return index;
}

int ListBox::getMaxLines() {
    return max_lines;
}

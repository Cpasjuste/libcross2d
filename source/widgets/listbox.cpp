//
// Created by cpasjuste on 05/01/18.
//

#include "cross2d/widgets/listbox.h"

using namespace c2d;

ListBoxLine::ListBoxLine(
        const FloatRect &rect, const std::string &str,
        Font *font, unsigned int fontSize, Texture *i, bool use_ic) : RectangleShape(rect) {

    //printf("ListBoxLine(%p)\n", this);

    icon = i;
    use_icons = use_ic;
    text = new Text(str, fontSize, font);
    text->setOutlineColor(Color::Black);
    text->setOrigin(Origin::Left);

    setFillColor(Color::Transparent);

    if (use_icons) {
        // add iconRect
        iconRect = new RectangleShape(Vector2f(32, 32));
        iconRect->setPosition(Vector2f(1, 1));
        iconRect->setFillColor(Color::Transparent);
        iconRect->setOutlineThickness(1);
        add(iconRect);
        // icon added in ListBox::setSelection (setIcon)
        // set text
        text->setPosition(iconRect->getSize().x + 8, getSize().y / 2);
        text->setSizeMax(getSize().x - 8 - iconRect->getSize().x, 0);
    } else {
        text->setPosition(8, getSize().y / 2);
        text->setSizeMax(getSize().x - 8, 0);
    }

    add(text);
}

void ListBoxLine::setSize(const Vector2f &size) {
    ListBoxLine::setSize(size.x, size.y);
}

void ListBoxLine::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
    text->setSizeMax(getSize().x - 8, 0);
}

void ListBoxLine::setString(const std::string &string) {
    text->setString(string);
}

void ListBoxLine::setColor(const Color &color) {

    text->setFillColor(color);
    if (iconRect) {
        iconRect->setOutlineColor(color);
    }
    if (icon) {
        icon->setOutlineColor(color);
    }
}

void ListBoxLine::setIcon(Texture *i) {

    if (use_icons) {
        if (icon) {
            remove(icon);
        }
        icon = i;
        if (icon) {
            if (icon->available) {
                icon->setOutlineThickness(1);
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
    //printf("~ListBoxLine(%p)\n", this);
}

ListBox::ListBox(Font *font, int fontSize, const FloatRect &rect,
                 const std::vector<Io::File> &fileList, bool useIcons)
        : RectangleShape(rect) {

    //printf("ListBox(%p)\n", this);

    setFiles(fileList);
    init(font, fontSize, useIcons);
    setSelection(0);
};

ListBox::ListBox(Font *font, int fontSize, const FloatRect &rect,
                 const std::vector<Io::File *> &fileList, bool useIcons)
        : RectangleShape(rect) {

    //printf("ListBox(%p)\n", this);

    setFiles(fileList);
    init(font, fontSize, useIcons);
    setSelection(0);
};

void ListBox::init(Font *font, int fontSize, bool useIcons) {

    use_icons = useIcons;
    // set default bg colors
    setFillColor(Color::GrayLight);

    // calculate number of lines shown
    if (use_icons) {
        line_height = 34; // 32px + 2px margin
    } else {
        line_height = fontSize + 10;
    }
    max_lines = (int) (getSize().y / line_height);
    if ((max_lines * line_height) < getSize().y) {
        line_height = getSize().y / (float) max_lines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape(Vector2f(getSize().x - 2, line_height - 2));
    add(highlight);

    // add lines of text
    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {
        FloatRect r = {1, (line_height * i) + 1, getSize().x - 2, line_height - 2};
        Texture *icon = nullptr;
        if (use_icons) {
            icon = files.size() > i ? files[i]->icon : nullptr;
        }
        ListBoxLine *line = new ListBoxLine(r, "", font, (unsigned int) fontSize, icon, use_icons);
        lines.push_back(line);
        add(line);
    }
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
            Io::File *file = files[index_start + i];
            lines[i]->setVisibility(Visibility::Visible);
            lines[i]->setString(file->name);
            // set text color based on file color
            lines[i]->setIcon(file->icon);
            lines[i]->setColor(file->color);
            // set highlight position and color
            if (index_start + i == (unsigned int) index) {
                highlight->setPosition(lines[i]->getPosition());
                Color color = highlight_use_files_color ?
                              file->color : highlight->getFillColor();
                color.a = highlight->getAlpha();
                highlight->setFillColor(color);
                color = highlight_use_files_color ?
                        file->color : highlight->getOutlineColor();
                //color.a = highlight->getAlpha();
                highlight->setOutlineColor(color);
            }
        }
    }

    if (files.empty()) {
        highlight->setVisibility(Visibility::Hidden, false);
    } else {
        if (use_highlight) {
            highlight->setVisibility(Visibility::Visible, false);
        }
    }
}

void ListBox::setSize(const Vector2f &size) {
    ListBox::setSize(size.x, size.y);
}

void ListBox::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
    highlight->setSize(width, highlight->getSize().y);
    for (auto &line : lines) {
        line->setSize(width, line->getSize().y);
    }
}

void ListBox::setFiles(const std::vector<Io::File> &fileList) {

    if (files_are_mine) {
        for (auto &file : files) {
            delete (file);
        }
    }
    files.clear();

    for (auto &file : fileList) {
        files.push_back(new Io::File(file));
    }

    files_are_mine = true;
    if (!lines.empty()) {
        setSelection(0);
    }
}

void ListBox::setFiles(const std::vector<Io::File *> fileList) {

    if (files_are_mine) {
        for (auto &file : files) {
            delete (file);
        }
    }
    files.clear();

    for (auto &file : fileList) {
        files.push_back(file);
    }

    files_are_mine = false;
    if (!lines.empty()) {
        setSelection(0);
    }
}

std::vector<c2d::Io::File *> ListBox::getFiles() {
    return files;
}

Io::File *ListBox::getSelection() {
    if (!files.empty() && files.size() > (unsigned int) index) {
        return files[index];
    }
    return nullptr;
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

void ListBox::setHighlightEnabled(bool enable) {
    use_highlight = enable;
    highlight->setVisibility(enable ? Visibility::Visible
                                    : Visibility::Hidden);
}

void ListBox::setHighlightUseFileColor(bool enable) {
    highlight_use_files_color = enable;
    setSelection(index);
}

int ListBox::getIndex() {
    return index;
}

int ListBox::getMaxLines() {
    return max_lines;
}

RectangleShape *ListBox::getHighlight() {
    return highlight;
}

ListBox::~ListBox() {
    // no need to delete lines widgets (ptr),
    // will be delete by parent (widget)
    //printf("~ListBox(%p)\n", this);
    if (files_are_mine) {
        for (auto &file : files) {
            delete (file);
        }
    }
}

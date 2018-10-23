//
// Created by cpasjuste on 19/10/18.
//

#include "cross2d/widgets/configbox.h"

using namespace c2d;
using namespace c2d::config;

ConfigBox::ConfigBox(Font *font, int fontSize, const c2d::FloatRect &rect)
        : RectangleShape(rect) {

    FloatRect leftRect = {0, 0, rect.width / 2, rect.height};
    FloatRect rightRect = {rect.width / 2, 0, rect.width / 2, rect.height};

    listBoxLeft = new ListBox(font, fontSize, leftRect);
    add(listBoxLeft);

    listBoxRight = new ListBox(font, fontSize, rightRect);
    add(listBoxRight);
}

ConfigBox::ConfigBox(Font *font, int fontSize, const c2d::Vector2f &size)
        : ConfigBox(font, fontSize, {0, 0, size.x, size.y}) {
}

ConfigBox::~ConfigBox() {
    delete (listBoxLeft);
    delete (listBoxRight);
}

void ConfigBox::load(config::Group *group) {

    if (!group) {
        return;
    }

    this->group = group;
    index = 0;

    std::vector<Io::File *> leftItems;
    std::vector<Io::File *> rightItems;

    for (Group &g : *group->getGroups()) {
        leftItems.push_back(new Io::File(g.getName(), ""));
        rightItems.emplace_back(new Io::File("ENTER", ""));
    }

    for (Option &option : *group->getOptions()) {
        leftItems.emplace_back(new Io::File(option.getName(), ""));
        rightItems.emplace_back(new Io::File(option.getString(), ""));
    }

    listBoxLeft->setFiles(leftItems);
    listBoxRight->setFiles(rightItems);
}

void ConfigBox::setLeftWidth(float width) {

    listBoxLeft->setSize(width, getSize().y);
    listBoxRight->setSize(getSize().x - width, getSize().y);
    listBoxRight->setPosition(width, 0);
}

ListBox *ConfigBox::getListBoxLeft() {
    return listBoxLeft;
}

ListBox *ConfigBox::getListBoxRight() {
    return listBoxRight;
}

Group *ConfigBox::getGroup() {
    return group;
}

config::Option *ConfigBox::getSelection() {
    Io::File *selection = listBoxLeft->getSelection();
    if (selection) {
        return group->getOption(selection->name);
    }
    return nullptr;
}

config::Option *ConfigBox::navigate(const ConfigBox::Navigation &navigation) {

    Option *selection = nullptr;

    if (navigation == Navigation::Up) {
        index--;
        if (index < 0)
            index = (int) (listBoxLeft->getFiles().size() - 1);
    } else if (navigation == Navigation::Down) {
        index++;
        if (index >= (int) listBoxLeft->getFiles().size()) {
            index = 0;
        }
        listBoxLeft->setSelection(index);
        listBoxRight->setSelection(index);
    } else if (navigation == Navigation::Left) {

    } else if (navigation == Navigation::Right) {

    } else if (navigation == Navigation::Enter) {
        if (getSelection()) {
            selection = getSelection();
        } else {
            Group *g = group->getGroup(listBoxLeft->getSelection()->name);
            if (g) {
                history.push_back(group);
                load(g);
                index = 0;
            }
        }
    } else if (navigation == Navigation::Exit) {
        if (!history.empty()) {
            Group *g = history.at(history.size() - 1);
            if (g) {
                history.pop_back();
                load(g);
                index = 0;
            }
        }
    }

    listBoxLeft->setSelection(index);
    listBoxRight->setSelection(index);

    return selection;
}

//
// Created by cpasjuste on 19/10/18.
//

#include "cross2d/widgets/configbox.h"

using namespace c2d;

ConfigBox::ConfigBox(const c2d::Vector2f &size) : RectangleShape(size) {

    //listBox = new ListBox()
}

ConfigBox::ConfigBox(const c2d::FloatRect &rect) : RectangleShape(rect) {

}

void ConfigBox::load(const config::Config &config) {

}

ListBox *ConfigBox::getListBox() {
    return listBox;
}

ConfigBox::~ConfigBox() {
    delete(listBox);
}

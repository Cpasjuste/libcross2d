//
// Created by cpasjuste on 17/10/18.
//

#include <cross2d/skeleton/option.h>

#include "cross2d/skeleton/option.h"

using namespace c2d;

c2d::Option::Option(const std::string &name,
                    const std::vector<std::string> &values,
                    int index, int id) {
    setName(name);
    setValues(values);
    setIndex(index);
    setId(id);
}

std::string c2d::Option::getName() {
    return name;
}

void Option::setName(const std::string &name) {
    this->name = name;
}

std::string c2d::Option::getInfo() {
    return info;
}

void Option::setInfo(const std::string &info) {
    this->info = info;
}

std::string c2d::Option::getValue() {
    if ((size_t) index >= values.size()) {
        return "<NA>";
    }
    return values[index];
}

std::vector<std::string> c2d::Option::getValues() {
    return values;
}

void Option::setValues(const std::vector<std::string> &values) {
    this->values = values;
    if (index >= values.size()) {
        index = 0;
    }
}

int Option::getIndex() {
    return index;
}

void Option::setIndex(int index) {
    if (index < values.size()) {
        this->index = index;
    }
}

int Option::getId() {
    return id;
}

void Option::setId(int id) {
    this->id = id;
}

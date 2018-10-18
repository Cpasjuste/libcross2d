//
// Created by cpasjuste on 17/10/18.
//

#include <algorithm>
#include <cross2d/skeleton/config_section.h>


#include "cross2d/skeleton/config_section.h"

using namespace c2d::config;

Section::Section(const std::string &name, int id) {

    this->name = name;
    this->id = id;
}

void Section::add(const Option &option) {

    options.push_back(option);
}

bool Section::remove(const std::string &name) {

    auto found = std::find_if(options.begin(), options.end(), [&name](Option const &option) {
        return name == option.getName();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
}

bool Section::remove(int id) {

    auto found = std::find_if(options.begin(), options.end(), [&id](Option const &item) {
        return id == item.getId();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
}

std::string Section::getName() const {

    return name;
}

int Section::getId() const {
    return id;
}

void Section::setId(int id) {
    this->id = id;
}

Option *Section::getOption(const std::string &name) {

    for (Option &option : options) {
        if (option.getName() == name) {
            return &option;
        }
    }

    return nullptr;
}

Option *Section::getOption(int id) {

    for (Option &option : options) {
        if (option.getId() == id) {
            return &option;
        }
    }

    return nullptr;
}

std::vector<Option> *Section::getOptions() {

    return &options;
}

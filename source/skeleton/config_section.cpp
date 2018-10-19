//
// Created by cpasjuste on 17/10/18.
//

#include <algorithm>

#include "cross2d/skeleton/config_section.h"

using namespace c2d::config;

Section::Section(const std::string &name, int id) {

    this->name = name;
    this->id = id;
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


/// childs options
void Section::addOption(const Option &option) {

    options.push_back(option);
}

bool Section::removeOption(const std::string &name) {

    auto found = std::find_if(options.begin(), options.end(), [&name](Option const &option) {
        return name == option.getName();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
}

bool Section::removeOption(int id) {

    auto found = std::find_if(options.begin(), options.end(), [&id](Option const &option) {
        return id == option.getId();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
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

#if 0       // TODO: handle recursive load/save in config
/// childs section
void Section::addSection(const Section &section) {

    sections.push_back(section);
}

bool Section::removeSection(const std::string &name) {

    auto found = std::find_if(sections.begin(), sections.end(), [&name](Section const &section) {
        return name == section.getName();
    });

    if (found != sections.end()) {
        sections.erase(found);
        return true;
    }

    return false;
}

bool Section::removeSection(int id) {

    auto found = std::find_if(sections.begin(), sections.end(), [&id](Section const &section) {
        return id == section.getId();
    });

    if (found != sections.end()) {
        sections.erase(found);
        return true;
    }

    return false;
}

Section *Section::getSection(const std::string &name) {

    for (Section &section : sections) {
        if (section.getName() == name) {
            return &section;
        }
    }

    return nullptr;
}

Section *Section::getSection(int id) {

    for (Section &section : sections) {
        if (section.getId() == id) {
            return &section;
        }
    }

    return nullptr;
}

std::vector<Section> *Section::getSections() {

    return &sections;
}
#endif

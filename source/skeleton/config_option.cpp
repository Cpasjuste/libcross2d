//
// Created by cpasjuste on 17/10/18.
//

#ifdef __DREAMCAST__

#include <cstdio>

#endif

#include "cross2d/skeleton/config_option.h"

using namespace c2d;
using namespace c2d::config;

Option::Option(const std::string &name, const std::string &value, int id) {
    setName(name);
    setId(id);
    setString(value);
    setType(Type::String);
}

Option::Option(const std::string &name, int value, int id) {
    setName(name);
    setId(id);
    setInteger(value);
    setType(Type::Integer);
}

Option::Option(const std::string &name, float value, int id) {
    setName(name);
    setId(id);
    setFloat(value);
    setType(Type::Float);
}

Option::Option(const std::string &name, const c2d::Vector2f &value, int id) {
    setName(name);
    setId(id);
    setVector2f(value);
    setType(Type::Vector2f);
}

Option::Option(const std::string &name, const c2d::FloatRect &value, int id) {
    setName(name);
    setId(id);
    setFloatRect(value);
    setType(Type::FloatRect);
}

Option::Option(const std::string &name, const Color &value, int id) {
    setName(name);
    setId(id);
    setColor(value);
    setType(Type::Color);
}

Option::Option(const std::string &name, const std::vector<std::string> &values, int index, int id) {
    setName(name);
    setId(id);
    setChoices(values, index);
    setType(Type::Choice);
}

std::string Option::getName() const {
    return name;
}

void Option::setName(const std::string &name) {
    this->name = name;
}

std::string Option::getComment() const {
    return info;
}

void Option::setComment(const std::string &info) {
    this->info = info;
}

int Option::getId() const {
    return id;
}

void Option::setId(int id) {
    this->id = id;
}

Option::Type Option::getType() const {
    return type;
}

void Option::setType(Option::Type type) {
    this->type = type;
}

bool Option::isSavable() const {
    return savable;
}

void Option::setSavable(bool s) {
    savable = s;
}

void *Option::getUserData() {
    return user_data;
}

void Option::setUserData(void *data) {
    user_data = data;
}

std::string Option::getString() const {
#ifdef __DREAMCAST__
    char str[128];
    if (type == Type::Integer) {
        snprintf(str, 127, "%i", integer);
        return std::string(str);
    } else if (type == Type::Float) {
        snprintf(str, 127, "%f", floatRect.left);
        return std::string(str);
    } else if (type == Type::Vector2f) {
        snprintf(str, 127, "%f, %f", floatRect.left, floatRect.top);
        return std::string(str);
    } else if (type == Type::FloatRect) {
        snprintf(str, 127, "%f, %f, %f, %f",
                 floatRect.left, floatRect.top, floatRect.width, floatRect.height);
        return std::string(str);
    } else if (type == Type::Color) {
        snprintf(str, 127, "%i, %i, %i, %i",
                 (int) floatRect.left, (int) floatRect.top, (int) floatRect.width, (int) floatRect.height);
        return std::string(str);
    } else if (type == Type::Choice) {
        return choices[choices_index];
    }
#else
    if (type == Type::Integer) {
        return std::to_string(integer);
    } else if (type == Type::Float) {
        return std::to_string(floatRect.left);
    } else if (type == Type::Vector2f) {
        return std::to_string(floatRect.left) + ", " + std::to_string(floatRect.top);
    } else if (type == Type::FloatRect) {
        return std::to_string(floatRect.left) + ", " + std::to_string(floatRect.top) + ", "
               + std::to_string(floatRect.width) + ", " + std::to_string(floatRect.height);
    } else if (type == Type::Color) {
        return std::to_string((int) floatRect.left) + ", " + std::to_string((int) floatRect.top) + ", "
               + std::to_string((int) floatRect.width) + ", " + std::to_string((int) floatRect.height);
    } else if (type == Type::Choice) {
        return choices[choices_index];
    }
#endif

    return string;
}

void Option::setString(const std::string &value) {
    string = value;
}

int Option::getInteger() {
    return integer;
}

void Option::setInteger(int value) {
    integer = value;
}

float Option::getFloat() {
    return floatRect.left;
}

void Option::setFloat(float value) {
    floatRect.left = value;
}

Vector2f Option::getVector2f() {
    return {floatRect.left, floatRect.top};
}

void Option::setVector2f(const Vector2f &value) {
    floatRect.left = value.x;
    floatRect.top = value.y;
}

FloatRect Option::getFloatRect() {
    return floatRect;
}

void Option::setFloatRect(const FloatRect &value) {
    floatRect = value;
}

Color Option::getColor() {
    return {(uint8_t) floatRect.left, (uint8_t) floatRect.top,
            (uint8_t) floatRect.width, (uint8_t) floatRect.height};
}

void Option::setColor(const Color &color) {
    floatRect.left = color.r;
    floatRect.top = color.g;
    floatRect.width = color.b;
    floatRect.height = color.a;
}

int Option::getChoiceIndex() {
    return choices_index;
}

void Option::setChoicesIndex(int index) {
    choices_index = index;
}

std::vector<std::string> Option::getChoices() {
    return choices;
}

void Option::setChoices(const std::vector<std::string> &values, int index) {
    choices = values;
    choices_index = index;
}

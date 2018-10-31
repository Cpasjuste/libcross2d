//
// Created by cpasjuste on 17/10/18.
//

#include "cross2d/skeleton/config_option.h"

using namespace c2d::config;

Option::Option(const std::string &name, const std::vector<std::string> &array, int id) {
    setName(name);
    setId(id);
    setType(Type::StringArray);
    setStringArray(array);
}

Option::Option(const std::string &name, const std::vector<int> &array, int id) {
    setName(name);
    setId(id);
    setType(Type::IntegerArray);
    //setStringArray(array);
}

Option::Option(const std::string &name, const std::vector<float> &array, int id) {
    setName(name);
    setId(id);
    setType(Type::FloatArray);
    //setString(value);
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

std::string Option::getString() const {

    if (type == Type::String) {
        if (!string_array.empty()) {
            return string_array[0];
        }
    } else if (type == Type::Integer) {
        if (!integer_array.empty()) {
            return std::to_string(integer_array[0]);
        }
    } else if (type == Type::Float) {
        if (!float_array.empty()) {
            return std::to_string(float_array[0]);
        }
    }

    return "";
}

std::vector<std::string> Option::getStringArray() const {

    std::vector<std::string> array;

    if (type == Type::String) {
        return string_array;
    } else if (type == Type::Integer) {
        for (int v : integer_array) {
            array.emplace_back(std::to_string(v));
        }
    } else if (type == Type::Float) {
        for (float v : float_array) {
            array.emplace_back(std::to_string(v));
        }
    }

    return array;
}

void Option::setString(const std::string &value) {
    if (!string_array.empty()) {
        string_array[0] = value;
    }
}

void Option::setStringArray(const std::vector<std::string> &array) {
    string_array = array;
}

int Option::getInteger() {
    return value_integer;
}

void Option::setInteger(int value) {
    value_integer = value;
}

void Option::setIntegerArray(const std::vector<int> &array) {

}

float Option::getFloat() {
    return value_float;
}

void Option::setFloat(float value) {
    value_float = value;
}

void Option::setFloatArray(const std::vector<float> &array) {

}

bool Option::isSavable() const {
    return savable;
}

void Option::setSavable(bool savable) {
    this->savable = savable;
}

void *Option::getUserData() {
    return user_data;
}

void Option::setUserData(void *data) {
    this->user_data = data;
}

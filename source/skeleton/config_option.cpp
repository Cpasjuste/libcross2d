//
// Created by cpasjuste on 17/10/18.
//
f
#include "cross2d/skeleton/config_option.h"

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
    if (type == Type::Integer) {
        return std::to_string(value_integer);
    } else if (type == Type::Float) {
        return std::to_string(value_float);
    }
    return value_string;
}

void Option::setString(const std::string &value) {
    value_string = value;
}

int Option::getInteger() {
    return value_integer;
}

void Option::setInteger(int value) {
    value_integer = value;
}

float Option::getFloat() {
    return value_float;
}

void Option::setFloat(float value) {
    value_float = value;
}

bool Option::isSavable() const {
    return savable;
}

void Option::setSavable(bool savable) {
    this->savable = savable;
}

void *Option::getUserData() {
    return data;
}

void Option::setUserData(void *data) {
    this->data = data;
}

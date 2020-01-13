//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "cross2d/c2d.h"

using namespace c2d;

std::string DreamCastIo::getHomePath() const {
    return Io::getHomePath();
}

bool DreamCastIo::exist(const std::string &path) {
    return  false;
}

bool DreamCastIo::create(const std::string &path) {
    return false;
}

size_t DreamCastIo::getSize(const std::string &file) {
    return 0;
}

Io::Type DreamCastIo::getType(const std::string &file) {
    return Type::Unknown;
}

std::vector<Io::File> DreamCastIo::getDirList(const std::string &path, bool sort, bool showHidden) {

    std::vector<Io::File> files;
    return files;
}

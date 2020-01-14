//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "cross2d/c2d.h"

using namespace c2d;

std::string DCIo::getHomePath() const {
    return Io::getHomePath();
}

bool DCIo::exist(const std::string &path) {
    return  false;
}

bool DCIo::create(const std::string &path) {
    return false;
}

size_t DCIo::getSize(const std::string &file) {
    return 0;
}

Io::Type DCIo::getType(const std::string &file) {
    return Type::Unknown;
}

std::vector<Io::File> DCIo::getDirList(const std::string &path, bool sort, bool showHidden) {

    std::vector<Io::File> files;
    return files;
}

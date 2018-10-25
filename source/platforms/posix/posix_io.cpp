//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#include "cross2d/platforms/posix/posix_io.h"

using namespace c2d;

std::string POSIXIo::getHomePath() const {

#ifndef __PSP2__
    char buf[1024];
    if (getcwd(buf, sizeof(buf))) {
        return std::string(buf) + "/";
    }
#endif
    return Io::getHomePath();
}

std::string POSIXIo::getDataPath() const {

    return getHomePath() + "data/";
}

bool POSIXIo::exist(const std::string &path) {
    struct stat st{};
    return (stat(path.c_str(), &st) == 0);
}

bool POSIXIo::create(const std::string &path) {
#ifdef __PSP2__
    return sceIoMkdir(path.c_str(), 0777) == 0;
#elif __WINDOWS__
    return mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

size_t POSIXIo::getSize(const std::string &file) {
    struct stat st{};
    if (stat(file.c_str(), &st) != 0) {
        return 0;
    }
    return (size_t) st.st_size;
}

Io::Type POSIXIo::getType(const std::string &file) {
    struct stat st{};
    if (stat(file.c_str(), &st) != 0) {
        return Type::Unknown;
    }
    return S_ISDIR(st.st_mode) ? Type::Directory : Type::File;
}

std::vector<Io::File> POSIXIo::getDirList(const std::string &path, bool sort) {

    std::vector<Io::File> files;
    struct dirent *ent;
    DIR *dir;

    if (!path.empty()) {
        if ((dir = opendir(path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                // skip "."
                if (strlen(ent->d_name) == 1 && ent->d_name[0] == '.') {
                    continue;
                }

                File file;
                file.name = ent->d_name;
                file.path = path + "/" + file.name;
                file.size = getSize(file.path);
                file.type = getType(file.path);
                file.color = file.type == Type::Directory ? Color::Yellow : Color::White;
                files.push_back(file);
            }
            closedir(dir);
            if (sort) {
                std::sort(files.begin(), files.end(), compare);
            }
        }
    }

    return files;
}

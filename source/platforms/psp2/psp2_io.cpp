//
// Created by cpasjuste on 19/12/16.
//

#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

#include "cross2d/platforms/psp2/psp2_io.h"

using namespace c2d;

bool PSP2Io::exist(const std::string &path) {
    struct stat st{};
    return (stat(path.c_str(), &st) == 0);
}

bool PSP2Io::create(const std::string &path) {
    return mkdir(path.c_str(), 0755) == 0;
}

size_t PSP2Io::getSize(const std::string &file) {
    struct stat st{};
    if (stat(file.c_str(), &st) != 0) {
        return 0;
    }
    return (size_t) st.st_size;
}

Io::Type PSP2Io::getType(const std::string &file) {
    struct stat st{};
    if (stat(file.c_str(), &st) != 0) {
        return Type::Unknown;
    }
    return S_ISDIR(st.st_mode) ? Type::Directory : Type::File;
}

std::vector<Io::File> PSP2Io::getDirList(const std::string &path, bool sort) {
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
                file.size = getSize(path);
                file.type = getType(path);
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

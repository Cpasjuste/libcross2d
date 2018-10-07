//
// Created by cpasjuste on 17/11/17.
//

#include <cstring>
#include <sys/stat.h>

#include "cross2d/platforms/posix/posix_io.h"

using namespace c2d;

bool POSIXIo::exist(const std::string &path) {
    struct stat st{};
    return (stat(path.c_str(), &st) == 0);
}

bool POSIXIo::create(const std::string &path) {
    return mkdir(path.c_str(), 0755) == 0;
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
                file.size = getSize(path);
                file.type = ent->d_type == DT_DIR ? Type::Directory : Type::File;
                file.color = ent->d_type == DT_DIR ? Color::Yellow : Color::White;
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

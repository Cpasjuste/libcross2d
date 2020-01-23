//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include "cross2d/c2d.h"

using namespace c2d;

std::string DCIo::getHomePath() {

    char buf[1024];
    if (getcwd(buf, sizeof(buf)) != nullptr) {
        std::string home(buf);
        if (!Utility::endsWith(home, "/")) {
            home += "/";
        }
        return home;
    }

    return Io::getHomePath();
}

bool DCIo::exist(const std::string &path) {

    file_t f = fs_open(path.c_str(), O_RDONLY);
    if (f != FILEHND_INVALID) {
        fs_close(f);
        return true;
    }

    f = fs_open(path.c_str(), O_DIR | O_RDONLY);
    if (f != FILEHND_INVALID) {
        fs_close(f);
        return true;
    }

    return false;
}

bool DCIo::create(const std::string &path) {

    return fs_mkdir(path.c_str()) == 0;
}

size_t DCIo::getSize(const std::string &file) {

    file_t f = fs_open(file.c_str(), O_RDONLY);
    if (f == FILEHND_INVALID) {
        return -1;
    }

    size_t size = fs_total(f);
    fs_close(f);

    return size;
}

Io::Type DCIo::getType(const std::string &file) {

    file_t f = fs_open(file.c_str(), O_DIR | O_RDONLY);
    if (f == FILEHND_INVALID) {
        return Type::File;
    }

    fs_close(f);

    return Type::Directory;
}

char *DCIo::read(const std::string &file) {

    file_t fd;
    ssize_t size;
    char *buffer = nullptr;

    fd = fs_open(file.c_str(), O_RDONLY);
    if (fd == FILEHND_INVALID) {
        printf("DCIo::read: can't open %s\n", file.c_str());
        return nullptr;
    }

    size = fs_total(fd);
    buffer = (char *) malloc(size);

    if (fs_read(fd, buffer, size) != size) {
        fs_close(fd);
        free(buffer);
        printf("DCIo::read: can't read %s\n", file.c_str());
        return nullptr;
    }

    fs_close(fd);

    return buffer;
}

bool DCIo::write(const std::string &file, const char *data) {

    file_t fd;

    fd = fs_open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY);
    if (fd == FILEHND_INVALID) {
        return false;
    }

    fs_write(fd, data, strlen(data));
    fs_close(fd);

    return true;
}

std::vector<Io::File> DCIo::getDirList(const std::string &path, bool sort, bool showHidden) {

    std::vector<Io::File> files;
    dirent_t *ent;
    file_t fd;

    if (path.empty()) {
        return files;
    }

    fd = fs_open(path.c_str(), O_RDONLY | O_DIR);
    if (fd == FILEHND_INVALID) {
        return files;
    }

    while ((ent = fs_readdir(fd)) != nullptr) {

        //printf("getDirList: %s\n", ent->name);

        // skip "."
        if (strlen(ent->name) == 1 && ent->name[0] == '.') {
            continue;
        }

        // skip ".." file in romdisk (?!)
        if (ent->attr != O_DIR && strlen(ent->name) == 2
            && ent->name[0] == '.' && ent->name[1] == '.') {
            continue;
        }

        // skip "hidden" files
        if (!showHidden && ent->name[0] == '.') {
            if (strlen(ent->name) != 2 && ent->name[1] != '.') {
                continue;
            }
        }

        File file;
        file.name = ent->name;
        file.path = Utility::removeLastSlash(path) + "/" + file.name;
        file.type = ent->attr == O_DIR ? Type::Directory : Type::File;
        file.color = file.type == Type::Directory ? Color::Yellow : Color::White;
        files.push_back(file);
    }

    fs_close(fd);

    if (sort) {
        std::sort(files.begin(), files.end(), compare);
    }

    return files;
}

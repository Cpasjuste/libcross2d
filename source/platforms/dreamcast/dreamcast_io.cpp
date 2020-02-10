//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include <dirent.h>
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

bool DCIo::removeFile(const std::string &path) {
    return unlink(path.c_str()) == 0;
}

bool DCIo::removeDir(const std::string &path) {

    struct dirent *ent;
    DIR *dir;

    File file = getFile(path);
    if (file.type != Type::Directory) {
        return false;
    }

    if ((dir = opendir(file.path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            std::string newPath =
                    file.path + (Utility::endsWith(file.path, "/") ? "" : "/") + std::string(ent->d_name);

            if (getType(newPath) == Type::Directory) {
                if (!removeDir(newPath)) {
                    closedir(dir);
                    return false;
                }
            } else {
                if (!removeFile(newPath)) {
                    closedir(dir);
                    return false;
                }
            }
        }
        closedir(dir);
    }

    return rmdir(path.c_str()) == 0;
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

        // skip some stuff
        if (std::string(ent->name) == "cd" || std::string(ent->name) == "ram" || std::string(ent->name) == "pty"
            || std::string(ent->name) == "rd" || std::string(ent->name) == "pc"
            || Utility::toLower(ent->name) == "recycler"
            || Utility::toLower(ent->name) == "$recycle.bin"
            || (Utility::toLower(ent->name) == "system volume information")) {
            continue;
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

Io::File DCIo::findFile(const std::string &path,
                        const std::vector<std::string> &whitelist, const std::string &blacklist) {

    File file;
    dirent_t *ent;
    file_t fd;

    if (path.empty()) {
        return file;
    }

    if ((fd = fs_open(path.c_str(), O_RDONLY | O_DIR)) != FILEHND_INVALID) {
        while ((ent = fs_readdir(fd)) != nullptr) {
            for (const auto &search : whitelist) {
                if (Utility::contains(ent->name, search)) {
                    if (!blacklist.empty() && Utility::contains(ent->name, blacklist)) {
                        continue;
                    }
                    file.name = ent->name;
                    file.path = Utility::removeLastSlash(path) + "/" + file.name;
                    file.type = ent->attr == O_DIR ? Type::Directory : Type::File;
                    file.color = file.type == Type::Directory ? Color::Yellow : Color::White;
                    break;
                }
            }
        }
        fs_close(fd);
    }

    return file;
}

bool DCIo::copy(const std::string &src, const std::string &dst,
                const std::function<void(File, File, float)> &callback) {

    bool res = _copy(src, dst, callback);

    if (callback != nullptr) {
        callback(File{}, File{}, res ? 2 : -1);
    }

    return res;
}

bool DCIo::_copy(const std::string &src, const std::string &dst,
                 const std::function<void(File, File, float)> &callback) {

    File srcFile;
    File dstFile;
    struct dirent *ent;
    DIR *dir;

    if (src == dst) {
        if (callback != nullptr) {
            callback(srcFile, dstFile, -1);
        }
        return false;
    }

    // The destination is a sub-folder of the source folder
    size_t size = src.size();
    if (src.compare(0, size, dst) == 0
        && (dst[size] == '/' || dst[size - 1] == '/')) {
        if (callback != nullptr) {
            callback(srcFile, dstFile, -1);
        }
        return false;
    }

    srcFile = getFile(src);
    dstFile = srcFile;
    dstFile.name = Utility::baseName(src);
    dstFile.path = dst;
    if (!Utility::endsWith(dstFile.path, "/")) {
        dstFile.path += "/";
    }
    dstFile.path += dstFile.name;

    if (srcFile.type == Type::File) {
        bool res = _copyFile(srcFile, dstFile, callback);
        if (callback != nullptr) {
            callback(srcFile, dstFile, 2);
        }
        return res;
    }

    if (!create(dstFile.path)) {
        if (callback != nullptr) {
            callback(srcFile, dstFile, -1);
        }
        return false;
    }

    if ((dir = opendir(srcFile.path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            std::string newSrcPath =
                    srcFile.path + (Utility::endsWith(dstFile.path, "/") ? "" : "/") + std::string(ent->d_name);
            File newSrcFile = getFile(newSrcPath);
            File newDstFile = newSrcFile;
            newDstFile.path = dstFile.path;

            if (newSrcFile.type == Type::File) {
                newDstFile.path += (Utility::endsWith(dstFile.path, "/") ? "" : "/") + std::string(ent->d_name);
                bool success = _copyFile(newSrcFile, newDstFile, callback);
                if (!success) {
                    if (callback != nullptr) {
                        callback(srcFile, dstFile, -1);
                    }
                    closedir(dir);
                    return false;
                }
            } else {
                bool success = _copy(newSrcFile.path, newDstFile.path, callback);
                if (!success) {
                    if (callback != nullptr) {
                        callback(srcFile, dstFile, -1);
                    }
                    closedir(dir);
                    return false;
                }
            }
        }
    }

    return true;
}

bool DCIo::_copyFile(const File &src, const File &dst,
                     const std::function<void(File, File, float)> &callback) {

    if (src.path == dst.path) {
        if (callback != nullptr) {
            callback(src, dst, -1);
        }
        return false;
    }

    // The destination is a sub-folder of the source folder
    size_t len = src.path.size();
    if (src.path.compare(0, len, dst.path) == 0
        && (dst.path[len] == '/' || dst.path[len - 1] == '/')) {
        if (callback != nullptr) {
            callback(src, dst, -1);
        }
        return false;
    }

    FILE *srcFd = fopen(src.path.c_str(), "r");
    if (srcFd == nullptr) {
        if (callback != nullptr) {
            callback(src, dst, -1);
        }
        return false;
    }

    FILE *dstFd = fopen(dst.path.c_str(), "w");
    if (dstFd == nullptr) {
        fclose(srcFd);
        if (callback != nullptr) {
            callback(src, dst, -1);
        }
        return false;
    }

    auto buf = (unsigned char *) malloc(C2D_IO_COPY_BUFFER_SIZE);
    if (buf == nullptr) {
        fclose(srcFd);
        fclose(dstFd);
        if (callback != nullptr) {
            callback(src, dst, -1);
        }
        return false;
    }

    if (callback != nullptr) {
        callback(src, dst, 0);
    }

    size_t readBytes, writeBytes, totalBytes = 0;
    while ((readBytes = fread(buf, 1, C2D_IO_COPY_BUFFER_SIZE, srcFd)) > 0) {
        writeBytes = fwrite(buf, 1, readBytes, dstFd);
        if (writeBytes < 0) {
            free(buf);
            fclose(srcFd);
            fclose(dstFd);
            if (callback != nullptr) {
                callback(src, dst, -1);
            }
            return false;
        }

        if (callback != nullptr) {
            totalBytes += writeBytes;
            float progress = (float) totalBytes / (float) src.size;
            callback(src, dst, progress);
        }
    }

    free(buf);
    fclose(srcFd);
    fclose(dstFd);

    return true;
}

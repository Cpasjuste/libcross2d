//
// Created by cpasjuste on 17/11/17.
//

#include <unistd.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#include "cross2d/c2d.h"

#ifdef __WINDOWS__
#define mkdir(x, y) mkdir(x)
#elif __PSP2__
#include <psp2/io/stat.h>
#define rmdir(x) sceIoRmdir(x)
#endif

using namespace c2d;

std::string POSIXIo::getHomePath() {
#if defined(__PSP2__) || defined(__RECALBOX__)
    return getDataPath();
#else
    char buf[1024];
    if (getcwd(buf, sizeof(buf)) != nullptr) {
        std::string str = buf;
        if (!Utility::endsWith(str, "/")) {
            str += "/";
        }
#ifdef __SWITCH__
        // some library does not like "sdmc:"
        if (str.compare(0, 5, "sdmc:") == 0) {
            str.erase(0, 5);
        }
#endif
        return str;
    }

    return Io::getHomePath();
#endif
}

Io::File POSIXIo::getFile(const std::string &path) {

    File file{};
    struct stat st{};

    if (stat(path.c_str(), &st) != 0) {
        return file;
    }

    file.name = Utility::baseName(path);
    file.path = path;
    file.size = (size_t) st.st_size;
    file.type = S_ISDIR(st.st_mode) ? Type::Directory : Type::File;

    return file;
}

bool POSIXIo::exist(const std::string &path) {
    if (path.empty()) {
        return false;
    }
    struct stat st{};
    return (stat(path.c_str(), &st) == 0);
}

bool POSIXIo::create(const std::string &path) {
#ifdef __PSP2__
    return sceIoMkdir(path.c_str(), 0777) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

bool POSIXIo::removeFile(const std::string &path) {
    return unlink(path.c_str()) == 0;
}

bool POSIXIo::removeDir(const std::string &path) {

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
    } else {
        return false;
    }

    return rmdir(path.c_str()) == 0;
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

char *POSIXIo::read(const std::string &file, size_t offset, size_t size) {

    FILE *fp = nullptr;
    size_t file_size;
    char *buffer = nullptr;

    fp = fopen(file.c_str(), "rb");
    if (fp == nullptr) {
        printf("POSIXIo::read: can't open %s\n", file.c_str());
        return nullptr;
    }

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    if (size <= 0) {
        size = file_size;
    }

    if (offset + size > file_size) {
        size = file_size - offset;
    }

    if (offset > 0) {
        fseek(fp, offset, SEEK_SET);
    }

    buffer = (char *) malloc(size);

    if (fread(buffer, 1, size, fp) != size) {
        fclose(fp);
        free(buffer);
        printf("POSIXIo::read: can't read %s\n", file.c_str());
        return nullptr;
    }

    fclose(fp);

    return buffer;
}

bool POSIXIo::write(const std::string &file, const char *data, size_t size) {

    FILE *fp;

    fp = fopen(file.c_str(), "w");
    if (fp == nullptr) {
        return false;
    }

    fwrite(data, 1, size, fp);
    fclose(fp);

    return true;
}

std::vector<Io::File> POSIXIo::getDirList(const std::string &path, bool sort, bool showHidden) {

    std::vector<Io::File> files;
    struct dirent *ent;
    DIR *dir;

    if (!path.empty()) {
        if ((dir = opendir(path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {

                //printf("getDirList: %s\n", ent->d_name);

                // skip "."
                if ((path == "/" || strlen(ent->d_name) == 1) && ent->d_name[0] == '.') {
                    continue;
                }
                // skip "hidden" files
                if (!showHidden && ent->d_name[0] == '.') {
                    if (strlen(ent->d_name) != 2 && ent->d_name[1] != '.') {
                        continue;
                    }
                }

                File file;
                file.name = ent->d_name;
                file.path = Utility::removeLastSlash(path) + "/" + file.name;
#if 0
                auto *dirSt = (fsdev_dir_t *) dir->dirData->dirStruct;
                FsDirectoryEntry *entry = &dirSt->entry_data[dirSt->index];
                file.type = entry->type == ENTRYTYPE_DIR ? Type::Directory : Type::File;
                file.size = entry->fileSize;
#else
                struct stat st{};
                if (stat(file.path.c_str(), &st) == 0) {
                    file.size = (size_t) st.st_size;
                    file.type = S_ISDIR(st.st_mode) ? Type::Directory : Type::File;
                }
#endif
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

Io::File POSIXIo::findFile(const std::string &path,
                           const std::vector<std::string> &whitelist, const std::string &blacklist) {

    struct dirent *ent;
    DIR *dir;
    File file{};

    if (path.empty()) {
        return file;
    }

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            for (const auto &search : whitelist) {
                if (Utility::contains(ent->d_name, search)) {
                    if (!blacklist.empty() && Utility::contains(ent->d_name, blacklist)) {
                        continue;
                    }
                    file.name = ent->d_name;
                    file.path = Utility::removeLastSlash(path) + "/" + file.name;
#if 0

                    auto *dirSt = (fsdev_dir_t *) dir->dirData->dirStruct;
                    FsDirectoryEntry *entry = &dirSt->entry_data[dirSt->index];
                    file.type = entry->type == ENTRYTYPE_DIR ? Type::Directory : Type::File;
                    file.size = entry->fileSize;
#else
                    struct stat st{};
                    if (stat(file.path.c_str(), &st) == 0) {
                        file.size = (size_t) st.st_size;
                        file.type = S_ISDIR(st.st_mode) ? Type::Directory : Type::File;
                    }
#endif
                    file.color = file.type == Type::Directory ? Color::Yellow : Color::White;
                    break;
                }
            }
        }
        closedir(dir);
    }

    return file;
}

bool POSIXIo::copy(const std::string &src, const std::string &dst,
                   const std::function<void(File, File, float)> &callback) {

    bool res = _copy(src, dst, callback);

    if (callback != nullptr) {
        callback(File{}, File{}, res ? 2 : -1);
    }

    return res;
}

bool POSIXIo::_copy(const std::string &src, const std::string &dst,
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
        closedir(dir);
    } else {
        return false;
    }

    return true;
}

bool POSIXIo::_copyFile(const File &src, const File &dst,
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

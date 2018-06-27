//
// Created by cpasjuste on 17/11/17.
//

#include <sys/stat.h>
#include "platforms/posix/posix_io.h"

using namespace c2d;

bool POSIXIo::exist(const std::string &path) {
    struct stat buf;
    return (stat(path.c_str(), &buf) == 0);
}

std::vector<std::string> POSIXIo::getDirList(const std::string &path) {

    std::vector<std::string> files;
    struct dirent *ent;
    DIR *dir;

    if (!path.empty()) {
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_REG) {
                    files.push_back(std::string(ent->d_name));
                }
            }
            closedir(dir);
        }
    }

    return files;
}

//
// Created by cpasjuste on 17/11/17.
//

#include <sys/stat.h>
#include "posix_io.h"

bool POSIXIo::Exist(const char *path) {
    struct stat buf;
    return (stat(path, &buf) == 0);
}

std::vector<std::string> POSIXIo::GetDirList(const char *path) {

    std::vector<std::string> files;
    struct dirent *ent;
    DIR *dir;

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                files.push_back(std::string(ent->d_name));
            }
        }
        closedir(dir);
    }

    return files;
}

//
// Created by cpasjuste on 19/12/16.
//

#include <dirent.h>
#include <sys/stat.h>
#include "platforms/psp2/psp2_io.h"

using namespace c2d;

bool PSP2Io::exist(const std::string &path) {

    struct stat buf;
    return (stat(path.c_str(), &buf) == 0);
}

std::vector <std::string> PSP2Io::getDirList(const std::string &path) {

    std::vector <std::string> files;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (SCE_S_ISREG(ent->d_stat.st_mode)) {
                files.push_back(std::string(ent->d_name));
            }
        }
        closedir(dir);
    }

    return files;
}

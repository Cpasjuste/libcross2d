//
// Created by cpasjuste on 17/11/17.
//

#include "../../include/nx/nx_io.h"

NXIo::NXIo() {

    Result res = fsInitialize();
    if (R_FAILED(res)) {
        fatalSimple(res);
    }

    res = fsMountSdcard(fs);
    if (R_FAILED(res)) {
        fatalSimple(res);
    }
}

NXIo::~NXIo() {

    fsFsClose(fs);
    fsExit();
}

bool NXIo::Exist(const char *path) {

    FsFile *file = NULL;
    Result res = fsFsOpenFile(fs, path, 0, file);
    if (R_FAILED(res)) {
        return false;
    } else {
        fsFileClose(file);
        return true;
    }
}

std::vector<std::string> NXIo::GetDirList(const char *path) {
    // TODO
    return Io::GetDirList(path);
}

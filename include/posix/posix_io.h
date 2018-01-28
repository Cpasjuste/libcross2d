//
// Created by cpasjuste on 17/11/17.
//

#ifndef PFBA_POSIX_IO_H
#define PFBA_POSIX_IO_H

#include <dirent.h>
#include "skeleton/io.h"

namespace c2d {

    class POSIXIo : Io {

    public:

        virtual bool exist(const char *path);

        virtual std::vector<std::string> getDirList(const char *path);

    };
}

#endif //PFBA_POSIX_IO_H

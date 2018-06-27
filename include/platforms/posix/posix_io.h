//
// Created by cpasjuste on 17/11/17.
//

#ifndef PFBA_POSIX_IO_H
#define PFBA_POSIX_IO_H

#include <dirent.h>
#include "skeleton/io.h"

namespace c2d {

    class POSIXIo : public Io {

    public:

        virtual bool exist(const std::string &path);

        virtual std::vector<std::string> getDirList(const std::string &path);

    };
}

#endif //PFBA_POSIX_IO_H

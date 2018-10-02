//
// Created by cpasjuste on 17/11/17.
//

#ifndef POSIX_IO_H
#define POSIX_IO_H

#include <dirent.h>
#include "skeleton/io.h"

namespace c2d {

    class POSIXIo : public Io {

    public:

        virtual bool exist(const std::string &path) override;

        virtual size_t getSize(const std::string &file) override;

        virtual Type getType(const std::string &file) override;

        virtual bool create(const std::string &path) override;

        virtual std::vector<Io::File> getDirList(const std::string &path, bool sort = false) override;

    };
}

#endif //POSIX_IO_H

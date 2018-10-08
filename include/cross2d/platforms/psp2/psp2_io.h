//
// Created by cpasjuste on 19/12/16.
//

#ifndef C2D_PSP2_IO_H
#define C2D_PSP2_IO_H

#include "cross2d/skeleton/io.h"

namespace c2d {

    class PSP2Io : public Io {

    public:

        virtual bool exist(const std::string &path) override;

        virtual size_t getSize(const std::string &file) override;

        virtual Type getType(const std::string &file) override;

        virtual bool create(const std::string &path) override;

        virtual std::vector<Io::File> getDirList(const std::string &path, bool sort = false) override;

    };
}

#endif //C2D_PSP2_IO_H

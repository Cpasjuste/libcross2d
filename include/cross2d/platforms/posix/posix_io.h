//
// Created by cpasjuste on 17/11/17.
//

#ifndef C2D_POSIX_IO_H
#define C2D_POSIX_IO_H

#include "cross2d/skeleton/io.h"

namespace c2d {

    class POSIXIo : public Io {

    public:

        bool exist(const std::string &path) override;

        size_t getSize(const std::string &file) override;

        Type getType(const std::string &file) override;

        bool create(const std::string &path) override;

        std::vector<Io::File> getDirList(const std::string &path, bool sort, bool showHidden) override;

        File findFile(const std::string &path,
                      const std::vector<std::string> &whitelist, const std::string blacklist) override;

        char *read(const std::string &file) override;

        bool write(const std::string &file, const char *data) override;

        std::string getHomePath() override;
    };
}

#endif //C2D_POSIX_IO_H

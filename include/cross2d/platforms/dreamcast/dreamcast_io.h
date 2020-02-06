//
// Created by cpasjuste on 17/11/17.
//

#ifndef C2D_DREAMCAST_IO_H
#define C2D_DREAMCAST_IO_H

#include "cross2d/skeleton/io.h"

namespace c2d {

    class DCIo : public Io {

    public:

        bool exist(const std::string &path) override;

        size_t getSize(const std::string &file) override;

        Type getType(const std::string &file) override;

        bool create(const std::string &path) override;

        bool remove(const std::string &path) override;

        bool copy(const std::string &src, const std::string &dst,
                  const std::function<void(File, File, int)> &callback = nullptr) override;

        std::vector<Io::File> getDirList(const std::string &path, bool sort, bool showHidden) override;

        File findFile(const std::string &path,
                      const std::vector<std::string> &whitelist, const std::string &blacklist) override;

        char *read(const std::string &file) override;

        bool write(const std::string &file, const char *data) override;

        std::string getHomePath() override;

        std::string getRomFsPath() override {
            return "/rd/";
        }

    private:
        bool copyFile(const File &src, const File &dst,
                      const std::function<void(File, File, int)> &callback = nullptr);
    };
}

#endif //C2D_DREAMCAST_IO_H

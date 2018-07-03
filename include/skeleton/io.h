//
// Created by cpasjuste on 17/11/17.
//

#ifndef CROSS2D_IO_H
#define CROSS2D_IO_H

#include <string>
#include <vector>
#include "skeleton/sfml/Color.hpp"
#include "skeleton/texture.h"

#define C2D_IO_FILE 0
#define C2D_IO_DIR  1

namespace c2d {

    class Io {

    public:

        class File {
        public:
            File() {};
            const char *name;
            const char *path;
            size_t size = 0;
            int type;
            // for ui
            Color color = Color::White;
            Texture *icon = nullptr;
        };

        Io() {};

        virtual ~Io() {};

        virtual bool exist(const std::string &path) { return false; };

        virtual bool createDir(const std::string &path) { return false; };

        virtual std::vector<std::string> getDirList(const std::string &path) { return std::vector<std::string>(); };

    };
}

#endif //CROSS2D_IO_H

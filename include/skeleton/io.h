//
// Created by cpasjuste on 17/11/17.
//

#ifndef CROSS2D_IO_H
#define CROSS2D_IO_H

#include <string>
#include <vector>
#include "skeleton/c2d_sfml/Color.hpp"

#define C2D_IO_FILE 0
#define C2D_IO_DIR  1

namespace c2d {

    class Io {

    public:

        class File {
        public:
            File() {};
            File(const std::string &n) { name = n; };
            std::string name;
            std::string path;
            size_t size = 0;
            int type;
            // for ui
            Color color = Color::White;
        };

        Io() {};

        virtual ~Io() {};

        virtual bool exist(const char *path) { return false; };

        virtual std::vector<std::string> getDirList(const char *path) { return {}; };

    };
}

#endif //CROSS2D_IO_H

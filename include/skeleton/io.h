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

        enum class Type {
            Unknown = 0,
            File = 1,
            Directory = 2
        };

        class File {
        public:
            File() {};
            std::string name;
            std::string path;
            size_t size = 0;
            Type type = Type::Unknown;
            // for ui
            Color color = Color::White;
            Texture *icon = nullptr;
        };

        Io() {};

        virtual ~Io() {};

        virtual bool exist(const std::string &path) { return false; };

        virtual size_t getSize(const std::string &file) { return 0; };

        virtual Type getType(const std::string &file) { return Type::Unknown; };

        virtual bool create(const std::string &path) { return false; };

        virtual std::vector<Io::File> getDirList(const std::string &path, bool sort = false) {
            return std::vector<Io::File>();
        };

    protected:

        static bool compare(const Io::File &a, const Io::File &b) {
            if (a.type == Type::Directory && b.type != Type::Directory) {
                return true;
            }
            if (a.type != Type::Directory && b.type == Type::Directory) {
                return false;
            }
            return a.name < b.name;
        }
    };
}

#endif //CROSS2D_IO_H

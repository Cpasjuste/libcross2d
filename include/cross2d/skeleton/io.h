//
// Created by cpasjuste on 17/11/17.
//

#ifndef CROSS2D_IO_H
#define CROSS2D_IO_H

#include <string>
#include <vector>
#include "cross2d/skeleton/sfml/Color.hpp"
#include "texture.h"
#include "utility.h"

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
            File() = default;

            File(const std::string &name, const std::string &path,
                 Type type = Type::File, const size_t &size = 0, const Color &color = Color::White) {
                this->name = name;
                this->path = path;
                this->type = type;
                this->size = size;
                this->color = color;
            }

            std::string name;
            std::string path;
            size_t size = 0;
            Type type = Type::Unknown;
            // for ui
            Color color = Color::White;
            Texture *icon = nullptr;
        };

        Io() = default;

        virtual ~Io() = default;

        ///
        /// \return home path
        virtual std::string getHomePath() const {
            return "./";
        }

        ///
        /// \return read only data path
        virtual std::string getDataReadPath() const {
            return getHomePath();
        }

        ///
        /// \return read write data path
        virtual std::string getDataWritePath() const {
            return getHomePath();
        }

        virtual bool exist(const std::string &path) {
            return false;
        };

        virtual size_t getSize(const std::string &file) {
            return 0;
        };

        virtual Type getType(const std::string &file) {
            return Type::Unknown;
        };

        virtual bool create(const std::string &path) {
            return false;
        };

        virtual std::vector<Io::File> getDirList(const std::string &path, bool sort = false, bool showHidden = false) {
            return std::vector<Io::File>();
        };

        static bool compare(const Io::File &a, const Io::File &b) {
            if (a.type == Type::Directory && b.type != Type::Directory) {
                return true;
            }
            if (a.type != Type::Directory && b.type == Type::Directory) {
                return false;
            }
            return Utility::toLower(a.name) < Utility::toLower(b.name);
        }
    };
}

#endif //CROSS2D_IO_H

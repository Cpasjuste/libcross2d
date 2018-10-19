//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_H
#define C2D_CONFIG_H

#include <string>
#include <vector>

#include "config_group.h"
#include "cross2d/skeleton/libconfig/libconfig.h"

namespace c2d {

    namespace config {

        class Config : public Group {

        public:

            Config(const std::string &name, const std::string &path, int version = 1);

            ~Config();

            bool load();

            bool save();

            int getVersion() const;

            void setVersion(int version);

            const std::string &getPath() const;

        private:

            int version;
            std::string path;
            config_t config;
        };
    }
}

#endif //C2D_CONFIG_H

//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_H
#define C2D_CONFIG_H

#include <string>
#include <vector>
#include <libconfig.h>

#include "config_group.h"

namespace c2d {

    namespace config {

        class Config : public Group {

        public:

            Config(const std::string &name, const std::string &path, int version = 1);

            bool load(const std::string &overridePath = {});

            bool loadFromString(const std::string &str);

            bool save();

            int getVersion();

            void setVersion(int version);

            std::string getPath();

        private:

            int version;
            std::string path;
        };
    }
}

#endif //C2D_CONFIG_H

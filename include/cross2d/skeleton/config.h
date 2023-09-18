//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_H
#define C2D_CONFIG_H

#include <string>
#include <vector>
#include <libconfig.h>

#include "config_group.h"

namespace c2d::config {
    class Config : public Group {
    public:
        Config(const std::string &name, const std::string &path, int version = 1);

        ~Config();

        bool load(const std::string &overridePath = {});

        bool loadFromString(const std::string &str);

        bool save();

        int getConfigVersion();

        int getAppVersion();

        std::string getPath();

    private:
        std::string m_path;
        config_t m_config{};
        int m_version = 0;
    };
}

#endif //C2D_CONFIG_H

//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_H
#define C2D_CONFIG_H

#include <string>
#include <vector>

#include "config_section.h"
#include "cross2d/skeleton/libconfig/libconfig.h"

namespace c2d {

    namespace config {

        class Config : public Section {

        public:

            Config(const std::string &name, const std::string &path,
                   int version = 1);

            Config(const std::string &name, const std::string &path,
                   int version, const std::vector<Section> &sections);

            ~Config();

            bool load();

            bool save();

            void addSection(const Section &section);

            Section *getSection(const std::string &name);

            Section *getSection(int id);

            std::vector<Section> *getSections();

            Option *getOption(const std::string &sectionName, const std::string &optionName);

            Option *getOption(int sectionId, int optionId);

            int getVersion() const;

            void setVersion(int version);

            const std::string &getPath() const;

        private:

            int version;
            std::string path;
            std::vector<config::Section> sections;
            config_t config;
        };
    }
}

#endif //C2D_CONFIG_H

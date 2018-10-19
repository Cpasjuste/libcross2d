//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_SECTION_H
#define C2D_CONFIG_SECTION_H

#include <string>
#include <vector>

#include "cross2d/skeleton/config_option.h"

namespace c2d {

    namespace config {

        class Section {

        public:

            Section(const std::string &name, int id = 0);

            std::string getName() const;

            int getId() const;

            void setId(int id);

            // child options
            void addOption(const Option &option);

            bool removeOption(const std::string &name);

            bool removeOption(int id);

            Option *getOption(const std::string &name);

            Option *getOption(int id);

            std::vector<Option> *getOptions();

#if 0       // TODO: handle recursive load/save in config
            // child sections
            void addSection(const Section &section);

            bool removeSection(const std::string &name);

            bool removeSection(int id);

            Section *getSection(const std::string &name);

            Section *getSection(int id);

            std::vector<Section> *getSections();
#endif
        private:

            std::string name;
            //std::vector<Section> sections;
            std::vector<Option> options;
            int id = 0;
        };
    }
}

#endif //C2D_CONFIG_SECTION_H

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
            Section(const std::string &name);

            void add(const Option &option);

            bool remove(const std::string &name);

            bool remove(int id);

            Option getOption(const std::string &name) const;

            Option getOption(int id) const;

            std::vector<Option> getOptions() const;

            std::string getName() const;

        private:

            std::string name;
            std::vector<Option> options;
        };
    }
}

#endif //C2D_CONFIG_SECTION_H

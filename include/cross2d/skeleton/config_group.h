//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_GROUP_H
#define C2D_CONFIG_GROUP_H

#include <string>
#include <vector>
#include <libconfig.h>

#include "cross2d/skeleton/config_option.h"

namespace c2d {

    namespace config {

        class Group {

        public:

            explicit Group(const std::string &name = "none", int id = -1);

            std::string getName() const;

            int getId() const;

            void setId(int id);

            // child options
            void addOption(const Option &option);

            bool removeOption(const std::string &name);

            bool removeOption(int id);

            Option *getOption(const std::string &name);

            Option *getOption(int id);

            Option *getOption(const std::string &groupName, const std::string &optionName);

            Option *getOption(int groupId, int optionId);

            std::vector<Option> *getOptions();

            // child groups
            void addGroup(const Group &group);

            bool removeGroup(const std::string &name);

            bool removeGroup(int id);

            Group *getGroup(const std::string &name);

            Group *getGroup(int id);

            std::vector<Group> *getGroups();

            bool isSavable() const;

            void setSavable(bool savable);

        protected:

            bool load(config_setting_t *root);

            bool save(config_setting_t *root);

        private:

            std::string name;
            std::vector<Group> groups;
            std::vector<Option> options;
            bool savable = true;
            int id = 0;
        };
    }
}

#endif //C2D_CONFIG_GROUP_H

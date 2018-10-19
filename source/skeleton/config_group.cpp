//
// Created by cpasjuste on 17/10/18.
//

#include <algorithm>
#include <cross2d/skeleton/config_group.h>


#include "cross2d/skeleton/config_group.h"

using namespace c2d::config;

Group::Group(const std::string &name, int id) {

    this->name = name;
    this->id = id;
}


std::string Group::getName() const {

    return name;
}

int Group::getId() const {
    return id;
}

void Group::setId(int id) {
    this->id = id;
}

/// childs options
void Group::addOption(const Option &option) {

    options.push_back(option);
}

bool Group::removeOption(const std::string &name) {

    auto found = std::find_if(options.begin(), options.end(), [&name](Option const &option) {
        return name == option.getName();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
}

bool Group::removeOption(int id) {

    auto found = std::find_if(options.begin(), options.end(), [&id](Option const &option) {
        return id == option.getId();
    });

    if (found != options.end()) {
        options.erase(found);
        return true;
    }

    return false;
}

Option *Group::getOption(const std::string &name) {

    for (Option &option : options) {
        if (option.getName() == name) {
            return &option;
        }
    }

    return nullptr;
}

Option *Group::getOption(int id) {

    for (Option &option : options) {
        if (option.getId() == id) {
            return &option;
        }
    }

    return nullptr;
}

Option *Group::getOption(const std::string &groupName, const std::string &optionName) {
    Group *group = getGroup(groupName);
    if (group) {
        return group->getOption(optionName);
    }
    return nullptr;
}

Option *Group::getOption(int groupId, int optionId) {
    Group *group = getGroup(groupId);
    if (group) {
        return group->getOption(optionId);
    }
    return nullptr;
}

std::vector<Option> *Group::getOptions() {

    return &options;
}

/// childs group
void Group::addGroup(const Group &group) {

    groups.push_back(group);
}

bool Group::removeGroup(const std::string &name) {

    auto found = std::find_if(groups.begin(), groups.end(), [&name](Group const &group) {
        return name == group.getName();
    });

    if (found != groups.end()) {
        groups.erase(found);
        return true;
    }

    return false;
}

bool Group::removeGroup(int id) {

    auto found = std::find_if(groups.begin(), groups.end(), [&id](Group const &group) {
        return id == group.getId();
    });

    if (found != groups.end()) {
        groups.erase(found);
        return true;
    }

    return false;
}

Group *Group::getGroup(const std::string &name) {

    if (name == this->name) {
        // used for recursive lookup
        return this;
    }

    for (Group &group : groups) {
        Group *s = group.getGroup(name);
        if (s) {
            return s;
        }
    }

    return nullptr;
}

Group *Group::getGroup(int id) {

    if (id == this->id) {
        // used for recursive lookup
        return this;
    }

    for (Group &group : groups) {
        Group *s = group.getGroup(id);
        if (s) {
            return s;
        }
    }

    return nullptr;
}

std::vector<Group> *Group::getGroups() {

    return &groups;
}

///
/// PROTECTED
///
bool Group::load(config_setting_t *parent) {

    if (!parent) {
        printf("Config::Group::load: could not find root config: %s\n", name.c_str());
        return false;
    }

    config_setting_t *settings = config_setting_lookup(parent, name.c_str());
    if (!settings) {
        printf("Config::Group::load: group not found, skipping: %s\n", name.c_str());
        return false;
    }

    for (auto &option : options) {
        if (option.getType() == Option::Type::String) {
            const char *value;
            if (!config_setting_lookup_string(settings, option.getName().c_str(), &value)) {
                printf("Config::Group::load: option not found, skipping: %s\n", option.getName().c_str());
                continue;
            }
            option.setString(value);
        } else if (option.getType() == Option::Type::Integer) {
            int value = 0;
            if (!config_setting_lookup_int(settings, option.getName().c_str(), &value)) {
                printf("Config::Group::load: option not found, skipping: %s\n", option.getName().c_str());
                continue;
            }
            option.setInteger(value);
        } else {
            // Option::Type::Float
            double value;
            if (!config_setting_lookup_float(settings, option.getName().c_str(), &value)) {
                printf("Config::Group::load: option not found, skipping: %s\n", option.getName().c_str());
                continue;
            }
            option.setFloat((float) value);
        }
    }

    // load childs, if any
    for (Group &group : groups) {
        group.load(settings);
    }

    return true;
}

bool Group::save(config_setting_t *parent) {

    if (!parent) {
        printf("Config::Group::save: could not save group (%s), parent is null\n", name.c_str());
        return false;
    }

    config_setting_t *root = config_setting_add(parent, name.c_str(), CONFIG_TYPE_GROUP);
    for (Option &option : options) {
        if (option.getType() == Option::Type::String) {
            config_setting_t *s = config_setting_add(root, option.getName().c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(s, option.getString().c_str());
        } else if (option.getType() == Option::Type::Integer) {
            config_setting_t *s = config_setting_add(root, option.getName().c_str(), CONFIG_TYPE_INT);
            config_setting_set_int(s, option.getInteger());
        } else {
            // Option::Type::Float
            config_setting_t *s = config_setting_add(root, option.getName().c_str(), CONFIG_TYPE_FLOAT);
            config_setting_set_float(s, (double) option.getFloat());
        }
    }

    // save childs, if any
    for (Group &group : groups) {
        group.save(root);
    }

    return true;
}

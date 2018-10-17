//
// Created by cpasjuste on 17/10/18.
//

#include <cross2d/skeleton/config.h>

#include "cross2d/skeleton/config.h"
#include "libconfig/libconfig.h"

using namespace c2d::config;

Config::Config(const std::string &name, const std::string &path, int version) {

    this->name = name;
    this->path = path;
    this->version = version;
    config_init(&config);
}

Config::Config(const std::string &name, const std::string &path, const std::vector<Section> &sections, int version) {

    this->name = name;
    this->path = path;
    this->sections = sections;
    this->version = version;
    config_init(&config);
}

bool Config::load() {

    printf("Config::load: %s\n", path.c_str());
    if (!config_read_file(&config, path.c_str())) {
        printf("Config::load: could not read file\n");
        return false;
    }

    config_setting_t *root = config_lookup(&config, name.c_str());
    if (!root) {
        printf("Config::load: could not find root config: %s\n", name.c_str());
        return false;
    }

    for (int i = 0; i < sections.size(); i++) {
        config_setting_t *settings = config_setting_lookup(root, sections[i].getName().c_str());
        if (!settings) {
            printf("Config::load: section not found, skipping: %s\n", sections[i].getName().c_str());
            continue;
        }
        for (int j = 0; j < sections[i].getOptions().size(); j++) {
            if (sections[i].getOption("").getType() == Option::Type::String) {
                const char *value;
                if (!config_setting_lookup_string(settings, option.getName().c_str(), &value)) {
                    printf("Config::load: option not found, skipping: %s\n", option.getName().c_str());
                    continue;
                }
                option.setString(value);
            } else if (option.getType() == Option::Type::Integer) {
                int value = 0;
                if (!config_setting_lookup_int(settings, option.getName().c_str(), &value)) {
                    printf("Config::load: option not found, skipping: %s\n", option.getName().c_str());
                    continue;
                }
                option.setInteger(value);
            } else {
                // Option::Type::Float
                double value;
                if (!config_setting_lookup_float(settings, option.getName().c_str(), &value)) {
                    printf("Config::load: option not found, skipping: %s\n", option.getName().c_str());
                    continue;
                }
                option.setFloat((float) value);
            }
        }
    }

    return true;
}

bool Config::save() {

    // create root
    config_setting_t *root = config_root_setting(&config);

    // create main group
    config_setting_t *setting_root = config_setting_add(root, name.c_str(), CONFIG_TYPE_GROUP);
    // add version
    config_setting_t *setting_version = config_setting_add(setting_root, "VERSION", CONFIG_TYPE_INT);
    config_setting_set_int(setting_version, version);

    for (auto &section : sections) {
        config_setting_t *setting = config_setting_add(setting_root, section.getName().c_str(), CONFIG_TYPE_GROUP);
        for (Option &option : section.getOptions()) {
            if (option.getType() == Option::Type::String) {
                config_setting_t *s = config_setting_add(setting, option.getName().c_str(), CONFIG_TYPE_STRING);
                config_setting_set_string(s, option.getString().c_str());
            } else if (option.getType() == Option::Type::Integer) {
                config_setting_t *s = config_setting_add(setting, option.getName().c_str(), CONFIG_TYPE_INT);
                config_setting_set_int(s, option.getInteger());
            } else {
                // Option::Type::Float
                config_setting_t *s = config_setting_add(setting, option.getName().c_str(), CONFIG_TYPE_FLOAT);
                config_setting_set_float(s, (double) option.getFloat());
            }
        }
    }

    return config_write_file(&config, path.c_str()) == 1;
}

void Config::add(const Section &section) {

    sections.push_back(section);
}

std::vector<Section> Config::getSections() const {
    return sections;
}

Section Config::getSection(const std::string &name) const {
    for (Section section : sections) {
        if (section.getName() == name) {
            return section;
        }
    }

    return Section("");
}

int Config::getVersion() const {
    return version;
}

void Config::setVersion(int version) {
    this->version = version;
}

const std::string &Config::getPath() const {
    return path;
}

const std::string &Config::getName() const {
    return name;
}

void Config::setName(const std::string &name) {
    this->name = name;
}

Config::~Config() {
    config_destroy(&config);
}


//
// Created by cpasjuste on 17/10/18.
//

#include "cross2d/skeleton/config.h"
#include "libconfig/libconfig.h"

using namespace c2d::config;

Config::Config(const std::string &name, const std::string &path, int version) : Section(name) {

    this->path = path;
    this->version = version;
    config_init(&config);
}

Config::Config(const std::string &name, const std::string &path, int version,
               const std::vector<Section> &sections) : Section(name) {

    this->path = path;
    this->sections = sections;
    this->version = version;
    config_init(&config);
}

bool Config::load() {

    printf("Config::load: %s\n", path.c_str());
    if (!config_read_file(&config, path.c_str())) {
        printf("Config::load: file not found: %s\n", path.c_str());
        return false;
    }

    config_setting_t *root = config_lookup(&config, getName().c_str());
    if (!root) {
        printf("Config::load: could not find root config: %s\n", getName().c_str());
        return false;
    }

#error  TODO: parse own options, recurse sections

    for (auto &section : sections) {
        config_setting_t *settings = config_setting_lookup(root, section.getName().c_str());
        if (!settings) {
            printf("Config::load: section not found, skipping: %s\n", section.getName().c_str());
            continue;
        }
        for (auto &option : *section.getOptions()) {
            if (option.getType() == Option::Type::String) {
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

    // generate new config
    config_destroy(&config);
    config_init(&config);

    // create root
    config_setting_t *root = config_root_setting(&config);

    // create main group
    config_setting_t *setting_root = config_setting_add(root, getName().c_str(), CONFIG_TYPE_GROUP);
    // add version
    config_setting_t *setting_version = config_setting_add(setting_root, "VERSION", CONFIG_TYPE_INT);
    config_setting_set_int(setting_version, version);

    for (auto &section : sections) {
        config_setting_t *setting = config_setting_add(setting_root, section.getName().c_str(), CONFIG_TYPE_GROUP);
        for (Option &option : *section.getOptions()) {
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

void Config::addSection(const Section &section) {

    sections.push_back(section);
}

Section *Config::getSection(const std::string &name) {
    for (Section &section : sections) {
        if (section.getName() == name) {
            return &section;
        }
    }
    return nullptr;
}

Section *Config::getSection(int id) {
    for (Section &section : sections) {
        if (section.getId() == id) {
            return &section;
        }
    }
    return nullptr;
}

std::vector<Section> *Config::getSections() {
    return &sections;
}

Option *Config::getOption(const std::string &sectionName, const std::string &optionName) {
    Section *section = getSection(sectionName);
    if (section) {
        return section->getOption(optionName);
    }
    return nullptr;
}

Option *Config::getOption(int sectionId, int optionId) {
    Section *section = getSection(sectionId);
    if (section) {
        return section->getOption(optionId);
    }
    return nullptr;
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

Config::~Config() {
    config_destroy(&config);
}

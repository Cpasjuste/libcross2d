//
// Created by cpasjuste on 17/10/18.
//

#include "cross2d/skeleton/config.h"

using namespace c2d::config;

Config::Config(const std::string &name, const std::string &path, int version) : Group(name) {
    this->path = path;
    this->version = version;
    config_init(&config);
}

bool Config::load(const std::string &overridePath) {
    std::string p = overridePath.empty() ? path : overridePath;
    printf("Config::load: %s\n", p.c_str());

    if (config_read_file(&config, p.c_str()) == CONFIG_FALSE) {
        printf("Config::load: file not found: %s\n", p.c_str());
        return false;
    }

    return Group::load(config_root_setting(&config));
}

bool Config::loadFromString(const std::string &str) {
    //printf("Config::loadFromString: %s\n", str.c_str());
    if (config_read_string(&config, str.c_str()) == CONFIG_FALSE) {
        printf("Config::loadFromString: error\n");
        return false;
    }

    return Group::load(config_root_setting(&config));
}

bool Config::save() {
    // generate new config
    config_destroy(&config);
    config_init(&config);

    // create root
    config_setting_t *root = config_root_setting(&config);

    /*
    // create main group
    config_setting_t *setting_root = config_setting_add(root, getName().c_str(), CONFIG_TYPE_GROUP);

    // add version (TODO)
    config_setting_t *setting_version = config_setting_add(setting_root, "VERSION", CONFIG_TYPE_INT);
    config_setting_set_int(setting_version, version);
    */
    Group::save(root);

    return config_write_file(&config, path.c_str()) == 1;
}

int Config::getVersion() {
    return version;
}

void Config::setVersion(int v) {
    version = v;
}

std::string Config::getPath() {
    return path;
}

Config::~Config() {
    config_destroy(&config);
}

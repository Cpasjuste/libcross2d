//
// Created by cpasjuste on 17/10/18.
//

#include "cross2d/skeleton/config.h"
#include "libconfig/libconfig.h"

using namespace c2d::config;

Config::Config(const std::string &name, const std::string &path, int version) : Group(name) {

    this->path = path;
    this->version = version;
    config_init(&config);
}

bool Config::load() {

    printf("Config::load: %s\n", path.c_str());
    if (!config_read_file(&config, path.c_str())) {
        printf("Config::load: file not found: %s\n", path.c_str());
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

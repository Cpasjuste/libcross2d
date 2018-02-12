//
// Created by cpasjuste on 27/01/17.
//

#include <SFML/Graphics/Shader.hpp>
#include "posix/posix_io.h"
#include "sfml/sfml_shaders.h"

using namespace c2d;

SFMLShaders::SFMLShaders(const std::string &shadersPath) : Shaders(shadersPath) {

    if (sf::Shader::isAvailable()) {
        POSIXIo io;
        std::vector<std::string> shaderList = io.GetDirList(shadersPath.c_str());
        for (unsigned int i = 0; i < shaderList.size(); i++) {
            const std::string name = shaderList[i].substr(0, shaderList[i].find_last_of("."));
            if (shaderList[i].substr(shaderList[i].find_last_of(".") + 1) == "v") { // vertex shader
                sf::Shader *shader = new sf::Shader();
                if (shader->loadFromFile(
                        shadersPath + "/" + name + ".v",
                        shadersPath + "/" + name + ".f")) {
                    add(name, shader);
                    //printf("SFMLShaders: %s loaded\n", name.c_str());
                } else {
                    delete (shader);
                }
            }
        }
    }

    printf("SFMLShaders: found %i shaders\n", getCount() - 1);
}

SFMLShaders::~SFMLShaders() {

    if (sf::Shader::isAvailable()) {
        for (int i = 0; i < getCount(); i++) {
            if (get(i)->data != NULL) {
                delete ((sf::Shader *) get(i)->data);
            }
        }
    }
}

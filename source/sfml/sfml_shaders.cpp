//
// Created by cpasjuste on 27/01/17.
//

#include <SFML/Graphics/Shader.hpp>
#include "../../include/posix/posix_io.h"
#include "../../include/sfml/sfml_shaders.h"

SFMLShaders::SFMLShaders(const std::string &shadersPath) : Shaders(shadersPath) {

    if (sf::Shader::isAvailable()) {
        POSIXIo io;
        std::vector<std::string> shaderList = io.GetDirList(shadersPath.c_str());
        for (int i = 0; i < shaderList.size(); i++) {
            const std::string name = shaderList[i].substr(0, shaderList[i].find_last_of("."));
            if (shaderList[i].substr(shaderList[i].find_last_of(".") + 1) == "v") { // vertex shader
                sf::Shader *shader = new sf::Shader();
                if (shader->loadFromFile(
                        shadersPath + "/" + name + ".v",
                        shadersPath + "/" + name + ".f")) {
                    Add(name, shader);
                    //printf("SFMLShaders: %s loaded\n", name.c_str());
                } else {
                    delete (shader);
                }
            }
        }
    }
    printf("SFMLShaders: found %i shaders\n", Count() - 1);
}

SFMLShaders::~SFMLShaders() {
    if (sf::Shader::isAvailable()) {
        for (int i = 0; i < Count(); i++) {
            if (Get(i)->data != NULL) {
                delete ((sf::Shader *) Get(i)->data);
            }
        }
    }
}

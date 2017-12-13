//
// Created by cpasjuste on 27/01/17.
//

#ifndef PFBA_SFML_SHADERS_H
#define PFBA_SFML_SHADERS_H

#include "../skeleton/shaders.h"

class SFMLShaders : Shaders {

public:
    SFMLShaders(const std::string &shadersPath);

    virtual ~SFMLShaders();
};


#endif //PFBA_SFML_SHADERS_H

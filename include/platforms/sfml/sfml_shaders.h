//
// Created by cpasjuste on 27/01/17.
//

#ifndef CROSS2D_SFML_SHADERS_H
#define CROSS2D_SFML_SHADERS_H

#include "skeleton/shaders.h"

namespace c2d {

    class SFMLShaders : Shaders {

    public:

        SFMLShaders(const std::string &shadersPath);

        virtual ~SFMLShaders();
    };
}

#endif //CROSS2D_SFML_SHADERS_H

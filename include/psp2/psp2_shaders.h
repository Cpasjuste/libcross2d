//
// Created by cpasjuste on 27/01/17.
//

#ifndef PFBA_PSP2_SHADERS_H
#define PFBA_PSP2_SHADERS_H

#include "skeleton/shaders.h"

namespace c2d {

    class PSP2Shaders : Shaders {

    public:

        PSP2Shaders(const std::string &shadersPath);

        virtual ~PSP2Shaders();
    };
}

#endif //PFBA_PSP2_SHADERS_H

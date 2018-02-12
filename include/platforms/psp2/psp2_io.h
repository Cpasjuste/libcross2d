//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_PSP2_UTILITY_H
#define FBA_PSP2_UTILITY_H

#include "skeleton/io.h"

namespace c2d {

    class PSP2Io : public Io {

    public:

        bool exist(const char *file);

        std::vector<std::string> getDirList(const char *path);

    };
}

#endif //FBA_PSP2_UTILITY_H

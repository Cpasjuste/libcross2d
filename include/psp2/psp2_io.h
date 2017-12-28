//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_PSP2_UTILITY_H
#define FBA_PSP2_UTILITY_H

#include "../skeleton/io.h"

class PSP2Io : Io {

public:
    bool Exist(const char *file);

    std::vector <std::string> GetDirList(const char *path);

};

#endif //FBA_PSP2_UTILITY_H

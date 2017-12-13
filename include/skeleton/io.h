//
// Created by cpasjuste on 17/11/17.
//

#ifndef PFBA_IO_H
#define PFBA_IO_H

#include <string>
#include <vector>

class Io {

public:

    Io() {};

    virtual ~Io() {};

    virtual bool Exist(const char *path) { return false; };

    virtual std::vector<std::string> GetDirList(const char *path) { return {}; };

};

#ifdef __PSP2__
#include "psp2/psp2_io.h"
#elif __3DS__
#include "posix/posix_io.h"
#elif __NX__
#include "nx/nx_io.h"
#elif __SDL2__ || __SDL1__
#include "../posix/posix_io.h"
#elif __SFML__
#include "posix/posix_io.h"
#endif

#endif //PFBA_IO_H

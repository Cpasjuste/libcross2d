//
// Created by cpasjuste on 17/11/17.
//

#ifndef PFBA_NX_IO_H
#define PFBA_NX_IO_H

#include <switch.h>
#include "../skeleton/io.h"

class NXIo : Io {

public:

    NXIo();

    virtual ~NXIo();

    virtual bool Exist(const char *path);

    virtual std::vector <std::string> GetDirList(const char *path);


private:

    FsFileSystem *fs;
};


#endif //PFBA_NX_IO_H

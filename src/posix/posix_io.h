//
// Created by cpasjuste on 17/11/17.
//

#ifndef PFBA_POSIX_IO_H
#define PFBA_POSIX_IO_H

#include <dirent.h>
#include <skeleton/io.h>

class POSIXIo : Io {

public:

    virtual bool Exist(const char *path);

    virtual std::vector<std::string> GetDirList(const char *path);

private:
};


#endif //PFBA_POSIX_IO_H

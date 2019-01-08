//
// Created by cpasjuste on 19/12/16.
//

#ifndef C2D_PSP2_IO_H
#define C2D_PSP2_IO_H

#include "cross2d/platforms/posix/posix_io.h"

namespace c2d {

    class PSP2Io : public POSIXIo {

    public:

        std::string getDataReadPath() const override {
            return "app0:/data/";
        }

        std::string getDataWritePath() const override {
            return "ux0:/data/";
        }
    };
}

#endif //C2D_PSP2_IO_H

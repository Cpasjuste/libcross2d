//
// Created by cpasjuste on 15/09/18.
//

#include <unistd.h>
#include "cross2d/c2d.h"

static int sock = -1;

void initNxLink() {
#ifndef NDEBUG
    socketInitializeDefault();
    sock = nxlinkStdio();
    if (sock < 0) {
        socketExit();
    }
#endif
}

void deinitNxLink() {
#ifndef NDEBUG
    if (sock >= 0) {
        close(sock);
        socketExit();
        sock = -1;
    }
#endif
}

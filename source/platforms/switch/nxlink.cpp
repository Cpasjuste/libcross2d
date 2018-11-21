//
// Created by cpasjuste on 15/09/18.
//

#include "cross2d/c2d.h"

#ifdef __NET_DEBUG__

#include <unistd.h>

static int sock = -1;

void initNxLink() {
    socketInitializeDefault();
    sock = nxlinkStdio();
    if (sock < 0) {
        socketExit();
    }
}

void deinitNxLink() {
    if (sock >= 0) {
        close(sock);
        socketExit();
        sock = -1;
    }
}

#endif // __NET_DEBUG__

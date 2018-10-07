//
// Created by cpasjuste on 15/09/18.
//

#include "cross2d/c2d.h"

#ifdef __NET_DEBUG__

#include <unistd.h>

int s_nxlinkSock = -1;

void initNxLink() {
    if (R_FAILED(socketInitializeDefault()))
        return;

    s_nxlinkSock = nxlinkStdio();
    if (s_nxlinkSock >= 0)
        printf("printf output now goes to nxlink server\n");
    else
        socketExit();
}

void deinitNxLink() {
    if (s_nxlinkSock >= 0) {
        close(s_nxlinkSock);
        socketExit();
        s_nxlinkSock = -1;
    }
}

/*
extern "C" void userAppInit() {
    initNxLink();
}

extern "C" void userAppExit() {
    deinitNxLink();
}
*/

#endif // __NET_DEBUG__

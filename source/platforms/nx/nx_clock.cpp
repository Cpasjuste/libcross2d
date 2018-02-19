//
// Created by cpasjuste on 11/02/18.
//

//#include <switch.h>
//#include <sys/time.h>
#include <switch.h>
#include "platforms/nx/nx_clock.h"

using namespace c2d;

#define TICKS_PER_MSEC 192000.000

NXClock::NXClock() : Clock() {

    m_startTime = getCurrentTime();
}

Time NXClock::getCurrentTime() const {

    //struct timeval time;
    //gettimeofday(&time, NULL);
    //return microseconds((long) 1000000 * time.tv_sec + time.tv_usec);
    return milliseconds((int) (svcGetSystemTick() / TICKS_PER_MSEC));
}

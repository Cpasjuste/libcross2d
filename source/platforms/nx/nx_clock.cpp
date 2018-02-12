//
// Created by cpasjuste on 11/02/18.
//

#include <sys/time.h>
#include "platforms/nx/nx_clock.h"

using namespace c2d;

NXClock::NXClock() : Clock() {

    m_startTime = getCurrentTime();
}

Time NXClock::getCurrentTime() const {

    struct timeval time;
    gettimeofday(&time, NULL);
    return microseconds((long) 1000000 * time.tv_sec + time.tv_usec);
}

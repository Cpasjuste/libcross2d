//
// Created by cpasjuste on 11/02/18.
//

#include <ctime>
#include "cross2d/platforms/dreamcast/dreamcast_clock.h"

using namespace c2d;

DreamCastClock::DreamCastClock() : Clock() {

    m_startTime = getCurrentTime();
}

Time DreamCastClock::getCurrentTime() const {

    /*
    struct timeval time{};
    gettimeofday(&time, nullptr);
    return microseconds((long) 1000000 * time.tv_sec + time.tv_usec);
    */

    // TODO:
    return microseconds(0);
}

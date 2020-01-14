//
// Created by cpasjuste on 11/02/18.
//

#include <arch/timer.h>
#include "cross2d/platforms/dreamcast/dreamcast_clock.h"

using namespace c2d;

DCClock::DCClock() : Clock() {

    m_startTime = getCurrentTime();
}

Time DCClock::getCurrentTime() const {

    uint64 time = timer_us_gettime64();
    return microseconds(time);
}

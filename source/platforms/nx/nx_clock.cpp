//
// Created by cpasjuste on 11/02/18.
//

#include <switch.h>
#include "platforms/nx/nx_clock.h"

using namespace c2d;

#define TICKS_PER_MSEC 19200.0000

NXClock::NXClock() : Clock() {

    m_startTime = getCurrentTime();
}

Time NXClock::getCurrentTime() const {

    return milliseconds((int) (svcGetSystemTick() / TICKS_PER_MSEC));
}

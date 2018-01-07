//
// Created by cpasjuste on 13/12/16.
//

#include <cstdio>
#include "timer.h"

Timer::Timer() {
    Reset();
}

void Timer::Reset() {
    gettimeofday(&start, NULL);
}

unsigned long Timer::GetMicros() {
    unsigned int ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (unsigned int) ((now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec);
    return ticks;
}

unsigned long Timer::GetMillis() {
    return GetMicros() / 1000;
}

unsigned int Timer::GetSeconds() {
    return (unsigned int) (GetMicros() / 1000000);
}

//
// Created by cpasjuste on 13/12/16.
//

#include <cstdio>
#include <sys/time.h>

#include "skeleton/timer.h"

Timer::Timer() {
    reset();
}

void Timer::reset() {
    gettimeofday(&start, NULL);
}

unsigned long Timer::getMicros() {
    unsigned int ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (unsigned int) ((now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec);
    return ticks;
}

unsigned long Timer::getMillis() {
    return getMicros() / 1000;
}

unsigned int Timer::getSeconds() {
    return (unsigned int) (getMicros() / 1000000);
}

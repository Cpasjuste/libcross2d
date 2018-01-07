//
// Created by cpasjuste on 13/12/16.
//

#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>

class Timer {

public:
    Timer();

    void Reset();

    unsigned long GetMicros();

    unsigned long GetMillis();

    unsigned int GetSeconds();

private:
    struct timeval start;
};

#endif //_TIMER_H_

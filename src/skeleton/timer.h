//
// Created by cpasjuste on 13/12/16.
//

#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>

class Timer {

public:
    Timer();

    virtual void Reset();

    virtual unsigned long GetMicros();

    virtual unsigned long GetMillis();

    virtual unsigned int GetSeconds();

private:
    struct timeval start;
};

#endif //_TIMER_H_

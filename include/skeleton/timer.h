//
// Created by cpasjuste on 13/12/16.
//

#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>

namespace c2d {

    class Timer {

    public:

        Timer();

        virtual ~Timer();

        virtual void reset();

        virtual unsigned long getMicros();

        virtual unsigned long getMillis();

        virtual unsigned int getSeconds();

    private:

        struct timeval start;
    };
}

#endif //_TIMER_H_

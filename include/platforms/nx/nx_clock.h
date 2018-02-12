//
// Created by cpasjuste on 11/02/18.
//

#ifndef CROSS2D_NX_CLOCK_H
#define CROSS2D_NX_CLOCK_H

#include "skeleton/sfml/Clock.hpp"

namespace c2d {

    class NXClock : public Clock {

    public:

        NXClock();

        Time getCurrentTime() const;
    };

}

#endif //CROSS2D_NX_CLOCK_H

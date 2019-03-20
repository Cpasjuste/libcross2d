//
// Created by cpasjuste on 11/01/17.
//

#ifndef _C2D_SWITCH_INPUT_H
#define _C2D_SWITCH_INPUT_H

#include "cross2d/platforms/sdl2/sdl2_input.h"

namespace c2d {

    class SWITCHInput : public SDL2Input {

    public:
        SWITCHInput();

        virtual Player *update(int rotate = 0) override;

        void setSingleJoyconMode(bool enable);

    private:
        bool single_joycon_mode = false;
    };
}

#endif //_C2D_SWITCH_INPUT_H

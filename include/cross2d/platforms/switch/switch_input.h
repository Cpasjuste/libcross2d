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

        Player *update(int rotate = 0) override;

        void setSingleJoyMode(bool enable);

    private:
        void process_axis(Input::Player &player, int rotate) override;

        void process_buttons(Input::Player &player, int rotate) override;

        PadState pad;
        int handheld_mode = 1;
        bool single_joy_mode = false;
        std::vector<ButtonMapping> mapLeft;
        std::vector<ButtonMapping> mapRight;
    };
}

#endif //_C2D_SWITCH_INPUT_H

//
// Created by cpasjuste on 11/01/17.
//

#ifndef _DREAMCAST_INPUT_H
#define _DREAMCAST_INPUT_H

#include "cross2d/skeleton/input.h"

namespace c2d {

    class DCInput : public Input {

    public:
        DCInput();

        virtual ~DCInput();

        virtual int waitButton(int player = 0);

        virtual Player *update(int rotate = 0);

    private:

        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_hat(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        virtual void process_keyboard(Input::Player &player, int rotate);

        struct InputData {
            maple_device_t *cont;
            cont_state_t *state;
        };
    };
}

#endif //_DREAMCAST_INPUT_H

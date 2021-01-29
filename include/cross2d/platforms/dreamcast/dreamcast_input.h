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

        ~DCInput() override;

        bool waitKey(unsigned int *key, int player = 0) override;

        Player *update(int rotate = 0) override;

        void setJoystickMapping(int player, const int *mapping, int deadzone = 8000) override;

    private:

        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        struct InputData {
            maple_device_t *cont;
            cont_state_t *state;
        };
    };
}

#endif //_DREAMCAST_INPUT_H

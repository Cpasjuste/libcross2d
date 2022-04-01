//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL2_INPUT_H
#define _SDL2_INPUT_H

#include "cross2d/skeleton/input.h"

namespace c2d {

    class SDL2Input : public Input {

    public:
        SDL2Input();

        ~SDL2Input() override;

        bool waitButton(unsigned int *key, int player = 0) override;

        Player *update(int rotate = 0) override;

    protected:

        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        virtual void process_keyboard(Input::Player &player, int rotate);

        virtual void process_touch(Input::Player &player);

#ifdef SDL2_INPUT_OLD_API
        virtual void process_hat(Input::Player &player, int rotate);
#endif
    };
}

#endif //_SDL2_INPUT_H

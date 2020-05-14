//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL1_INPUT_H
#define _SDL1_INPUT_H

#include "cross2d/skeleton/input.h"

namespace c2d {

    class SDL1Input : public Input {

    public:
        SDL1Input();

        ~SDL1Input() override;

        bool waitKey(unsigned int *key, int player = 0) override;

        Player *update(int rotate = 0) override;

    private:
        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_hat(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        virtual void process_keyboard(Input::Player &player, int rotate);

    };
}

#endif //_SDL1_INPUT_H

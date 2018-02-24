//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL2_INPUT_H
#define _SDL2_INPUT_H

#include "skeleton/input.h"
#include <SDL2/SDL.h>

namespace c2d {

    class SDL2Input : public Input {

    public:
        SDL2Input(Renderer *renderer);

        virtual ~SDL2Input();

        virtual Player *update(int rotate = 0);

        virtual int getButton(int player = 0);

    private:
        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_hat(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        virtual void process_keyboard(Input::Player &player, int rotate);

    };
}

#endif //_SDL2_INPUT_H

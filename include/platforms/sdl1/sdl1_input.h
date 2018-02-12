//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL1_INPUT_H
#define _SDL1_INPUT_H

#include "skeleton/input.h"
#include "skeleton/renderer.h"
#include <SDL/SDL.h>

namespace c2d {

    class SDL1Input : Input {

    public:
        SDL1Input(Renderer *renderer);

        virtual ~SDL1Input();

        virtual Player *update(int rotate);

        virtual int getButton(int player);

    private:
        virtual void process_axis(Input::Player &player, int rotate);

        virtual void process_hat(Input::Player &player, int rotate);

        virtual void process_buttons(Input::Player &player, int rotate);

        virtual void process_keyboard(Input::Player &player, int rotate);

    };
}

#endif //_SDL1_INPUT_H

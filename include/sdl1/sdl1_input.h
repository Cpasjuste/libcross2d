//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL1_INPUT_H
#define _SDL1_INPUT_H

#include "../skeleton/input.h"
#include <SDL/SDL.h>

class SDL1Input : Input {

public:
    SDL1Input(Renderer *renderer);

    virtual ~SDL1Input();

    virtual Player *Update(int rotate);

    virtual int GetButton(int player);

private:
    virtual void process_axis(Input::Player &player, int rotate);

    virtual void process_hat(Input::Player &player, int rotate);

    virtual void process_buttons(Input::Player &player, int rotate);

    virtual void process_keyboard(Input::Player &player, int rotate);

};

#endif //_SDL1_INPUT_H

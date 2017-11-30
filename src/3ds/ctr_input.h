//
// Created by cpasjuste on 11/01/17.
//

#ifndef _CTR_INPUT_H
#define _CTR_INPUT_H

#include <skeleton/renderer.h>
#include <skeleton/input.h>

class CTRInput : Input {

public:
    CTRInput(Renderer *renderer);

    virtual ~CTRInput();

    virtual Player *Update(int rotate = 0);

    virtual int GetButton(int player);

private:
    virtual void process_buttons(Input::Player &player, int rotate = 0);

};

#endif //_CTR_INPUT_H

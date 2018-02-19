//
// Created by cpasjuste on 11/01/17.
//

#ifndef CROSS2D_NX_INPUT_H
#define CROSS2D_NX_INPUT_H

#include "skeleton/renderer.h"
#include "skeleton/input.h"

namespace c2d {

    class NXInput : public Input {

    public:
        NXInput(Renderer *renderer);

        virtual ~NXInput();

        virtual Player *update(int rotate = 0);

        virtual int clear(int player);

        virtual int getButton(int player);

    private:
        virtual void process_buttons(Input::Player &player, int rotate = 0);

    };
}

#endif //CROSS2D_NX_INPUT_H

//
// Created by cpasjuste on 11/01/17.
//

#ifndef C2D_CTR_INPUT_H
#define C2D_CTR_INPUT_H

#include "cross2d/skeleton/renderer.h"
#include "cross2d/skeleton/input.h"

namespace c2d {

    class CTRInput : public Input {

    public:

        explicit CTRInput();

        ~CTRInput() override;

        Player *update(int rotate = 0) override;

        bool waitKey(unsigned int *key, int player = 0) override;

    private:
        void process_buttons(Input::Player &player, int rotate = 0);

    };
}

#endif //C2D_CTR_INPUT_H

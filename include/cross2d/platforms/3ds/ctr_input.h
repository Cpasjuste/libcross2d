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

        Player *update() override;

        // TODO: int waitButton(int player = 0) override;

    private:
        // TODO: Vector2f getAxisState(const Player &player, int xAxis, int yAxis) override;

        int getButtonState(const Player &player, int button) override;

        // TODO: int getKeyState(int key) override;

        Vector2f getTouch() override;
    };
}

#endif //C2D_CTR_INPUT_H

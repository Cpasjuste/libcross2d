//
// Created by cpasjuste on 11/01/17.
//

#include <3ds.h>
#include "cross2d/platforms/3ds/ctr_input.h"

using namespace c2d;

CTRInput::CTRInput() : Input() {
    m_players[0].enabled = true;
}

Input::Player *CTRInput::update() {
    hidScanInput();
    Player *players = Input::update();

    if (!aptMainLoop()) {
        players[0].buttons |= Input::Button::Quit;
        return players;
    }

    return players;
}

int CTRInput::getButtonState(const Player &player, int button) {
    return (hidKeysHeld() & (u32) button) > 0;
}

Vector2f CTRInput::getTouch() {
    touchPosition touch;
    hidTouchRead(&touch);
    return {touch.px, touch.py};
}

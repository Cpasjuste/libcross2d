//
// Created by cpasjuste on 13/01/17.
//

#include <skeleton/input.h>

#include "c2d.h"

using namespace c2d;

Input::Input(Renderer *r) : keyboard() {

    this->renderer = r;
    repeatClock = new C2DClock();
}

Input::Player *Input::update(int rotate) {

    float elapsed = repeatClock->getElapsedTime().asMilliseconds();

    if (!repeat || players[0].state & EV_QUIT) {
        stateOld = players[0].state;
        return players;
    }

    if (elapsed >= repeatDelay) {
        repeatClock->restart();
        stateOld = players[0].state;
        return players;
    } else {
        unsigned int diff = stateOld ^players[0].state;
        stateOld = players[0].state;
        if (diff > 0) {
            repeatClock->restart();
        } else {
            players[0].state = 0;
        }
    }

    return players;
}

void Input::setRepeatEnable(bool enable) {
    repeat = enable;
}

void Input::setRepeatDelay(int ms) {
    repeatDelay = ms;
}

int Input::clear(int player) {
    while (true) {
        Player p = update(0)[player];
        if (!p.enabled || !p.state
            || p.state & EV_REFRESH || p.state & EV_QUIT) {
            break;
        }
    }

    return 0;
}

void Input::setJoystickMapping(int player, const int *mapping, int deadzone) {
    for (int i = 0; i < KEY_COUNT; i++) {
        players[player].mapping[i] = mapping[i];
    }
    players[player].dead_zone = deadzone;
}

void Input::setKeyboardMapping(const int *mapping) {
    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = mapping[i];
    }
}

Input::~Input() {
    delete (repeatClock);
}

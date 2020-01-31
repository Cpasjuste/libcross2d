//
// Created by cpasjuste on 13/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

Input::Input() : keyboard() {

    repeatClock = new C2DClock();
}

Input::Player *Input::update(int rotate) {

    int elapsed = repeatClock->getElapsedTime().asMilliseconds();

    if (!repeat || players[0].keys & EV_QUIT) {
        stateOld = players[0].keys;
        return players;
    }

    if (elapsed >= repeatDelay) {
        repeatClock->restart();
        stateOld = players[0].keys;
        return players;
    } else {
        unsigned int diff = stateOld ^players[0].keys;
        stateOld = players[0].keys;
        if (diff > 0) {
            repeatClock->restart();
        } else {
            players[0].keys = Key::Delay;
        }
    }

    return players;
}

void Input::setRepeatDelay(int ms) {
    repeatDelay = ms;
    repeat = ms > 0;
}

int Input::getRepeatDelay() {
    return repeatDelay;
}

int Input::clear(int player) {
    while (true) {
        Player p = update(0)[player];
        if (!p.enabled || !p.keys || p.keys & EV_QUIT) {
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

unsigned int Input::getKeys(int player) {
    if (player < PLAYER_MAX) {
        return players[player].keys;
    }
    return 0;
}

Input::Player *Input::getPlayer(int player) {
    if (player < PLAYER_MAX) {
        return &players[player];
    }
    return nullptr;
}

Input::Player *Input::getPlayers() {
    return players;
}

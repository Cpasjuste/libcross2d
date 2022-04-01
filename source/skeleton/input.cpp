//
// Created by cpasjuste on 13/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

Input::Input() {
    // set default joystick mapping
    std::vector<c2d::Input::ButtonMapping> joy_mapping = {
            {Button::Up,     KEY_JOY_UP_DEFAULT},
            {Button::Down,   KEY_JOY_DOWN_DEFAULT},
            {Button::Left,   KEY_JOY_LEFT_DEFAULT},
            {Button::Right,  KEY_JOY_RIGHT_DEFAULT},
            {Button::Select, KEY_JOY_SELECT_DEFAULT},
            {Button::Start,  KEY_JOY_START_DEFAULT},
            {Button::A,      KEY_JOY_A_DEFAULT},
            {Button::B,      KEY_JOY_B_DEFAULT},
            {Button::X,      KEY_JOY_X_DEFAULT},
            {Button::Y,      KEY_JOY_Y_DEFAULT},
            {Button::LT,     KEY_JOY_LT_DEFAULT},
            {Button::RT,     KEY_JOY_RT_DEFAULT},
            {Button::LB,     KEY_JOY_LB_DEFAULT},
            {Button::RB,     KEY_JOY_RB_DEFAULT},
            {Button::LS,     KEY_JOY_LS_DEFAULT},
            {Button::RS,     KEY_JOY_RS_DEFAULT},
            {Button::Menu1,  KEY_JOY_MENU1_DEFAULT},
            {Button::Menu2,  KEY_JOY_MENU2_DEFAULT}
    };
    for (int i = 0; i < PLAYER_MAX; i++) {
        Input::setJoystickMapping(
                i, joy_mapping,
                KEY_JOY_AXIS_LX, KEY_JOY_AXIS_LY, KEY_JOY_AXIS_RX, KEY_JOY_AXIS_RY, 8000);
    }

#ifndef NO_KEYBOARD
    // set default keyboard mapping
    std::vector<c2d::Input::ButtonMapping> kb_mapping = {
            {Button::Up,     KEY_KB_UP_DEFAULT},
            {Button::Down,   KEY_KB_DOWN_DEFAULT},
            {Button::Left,   KEY_KB_LEFT_DEFAULT},
            {Button::Right,  KEY_KB_RIGHT_DEFAULT},
            {Button::Select, KEY_KB_SELECT_DEFAULT},
            {Button::Start,  KEY_KB_START_DEFAULT},
            {Button::A,      KEY_KB_A_DEFAULT},
            {Button::B,      KEY_KB_B_DEFAULT},
            {Button::X,      KEY_KB_X_DEFAULT},
            {Button::Y,      KEY_KB_Y_DEFAULT},
            {Button::LT,     KEY_KB_LT_DEFAULT},
            {Button::RT,     KEY_KB_RT_DEFAULT},
            {Button::LB,     KEY_KB_LB_DEFAULT},
            {Button::RB,     KEY_KB_RB_DEFAULT},
            {Button::LS,     KEY_KB_LS_DEFAULT},
            {Button::RS,     KEY_KB_RS_DEFAULT},
            {Button::Menu1,  KEY_KB_MENU1_DEFAULT},
            {Button::Menu2,  KEY_KB_MENU2_DEFAULT}
    };
    Input::setKeyboardMapping(kb_mapping);
#endif

    // auto repeat timer
    repeatClock = new C2DClock();
}

Input::Player *Input::update(int rotate) {
    int elapsed = repeatClock->getElapsedTime().asMilliseconds();

    if (!repeat || players[0].buttons & Input::Button::Quit) {
        stateOld = players[0].buttons;
        return players;
    }

    if (elapsed >= repeatDelay) {
        repeatClock->restart();
        stateOld = players[0].buttons;
        return players;
    } else {
        unsigned int diff = stateOld ^ players[0].buttons;
        stateOld = players[0].buttons;
        if (diff > 0) {
            repeatClock->restart();
        } else {
            players[0].buttons = Button::Delay;
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
        if (!p.enabled || p.buttons == 0 || p.buttons & Input::Button::Quit) {
            break;
        }
    }

    return 0;
}

void Input::setJoystickMapping(int player, const std::vector<ButtonMapping> &mapping,
                               int lx, int ly, int rx, int ry, int dz) {
    if (player < PLAYER_MAX) {
        players[player].mapping = mapping;
        players[player].lx.id = lx;
        players[player].ly.id = ly;
        players[player].rx.id = rx;
        players[player].ry.id = ry;
        players[player].dz = dz;
    }
}

void Input::setKeyboardMapping(const std::vector<ButtonMapping> &mapping) {
    keyboard.mapping = mapping;
}

unsigned int Input::getButtons(int player) {
    if (player < PLAYER_MAX) {
        return players[player].buttons;
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

Input::~Input() {
    delete (repeatClock);
}
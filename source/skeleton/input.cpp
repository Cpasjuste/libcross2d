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
                {KEY_JOY_AXIS_LX, KEY_JOY_AXIS_LY},
                {KEY_JOY_AXIS_RX, KEY_JOY_AXIS_RY}, 8000);
        players[i].mapping_default = joy_mapping;
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
    keyboard.mapping_default = kb_mapping;
#endif

    // auto repeat timer
    m_repeatClock = new C2DClock();
}

Input::Player *Input::update() {
    int elapsed = m_repeatClock->getElapsedTime().asMilliseconds();

    for (auto &player: players) {
        if (!player.enabled || !player.data) {
            continue;
        }

        // reset buttons state
        player.buttons = 0;

        /// process axis (@ rsn8887)
        Vector2f analogState;
        auto deadZone = (float) player.dz;
        float scalingFactor, magnitude;
        Axis *currentStickXAxis;
        Axis *currentStickYAxis;
        float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones

        // process directional axis
        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                // left stick
                currentStickXAxis = &(player.lx);
                currentStickYAxis = &(player.ly);
            } else {
                // right stick
                currentStickXAxis = &(player.rx);
                currentStickYAxis = &(player.ry);
            }

            analogState = getAxisState(player, currentStickXAxis->id, currentStickYAxis->id);

            // radial and scaled deadzone
            // http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html

            if ((magnitude = std::sqrt(analogState.x * analogState.x + analogState.y * analogState.y)) >= deadZone) {
                // analog control
                scalingFactor = 32767.0f / magnitude * (magnitude - deadZone) / (32769.0f - deadZone);
                currentStickXAxis->value = (short) (analogState.x * scalingFactor);
                currentStickYAxis->value = (short) (analogState.y * scalingFactor);

                // symmetric angular zones for all eight digital directions
                analogState.y = -analogState.y;
                if (analogState.y > 0 && analogState.x > 0) {
                    // upper right quadrant
                    if (analogState.y > slope * analogState.x)
                        player.buttons |= Input::Button::Up;
                    if (analogState.x > slope * analogState.y)
                        player.buttons |= Input::Button::Right;
                } else if (analogState.y > 0 && analogState.x <= 0) {
                    // upper left quadrant
                    if (analogState.y > slope * (-analogState.x))
                        player.buttons |= Input::Button::Up;
                    if ((-analogState.x) > slope * analogState.y)
                        player.buttons |= Input::Button::Left;
                } else if (analogState.y <= 0 && analogState.x > 0) {
                    // lower right quadrant
                    if ((-analogState.y) > slope * analogState.x)
                        player.buttons |= Input::Button::Down;
                    if (analogState.x > slope * (-analogState.y))
                        player.buttons |= Input::Button::Right;
                } else if (analogState.y <= 0 && analogState.x <= 0) {
                    // lower left quadrant
                    if ((-analogState.y) > slope * (-analogState.x))
                        player.buttons |= Input::Button::Down;
                    if ((-analogState.x) > slope * (-analogState.y))
                        player.buttons |= Input::Button::Left;
                }
            } else {
                currentStickXAxis->value = 0;
                currentStickYAxis->value = 0;
            }
        }

        /// process buttons
        for (const auto &buttonMap: player.mapping) {
            if (getButtonState(player, buttonMap.value)) {
                player.buttons |= getButtonRotation(buttonMap.button);
            }
        }
    }

    /// process keyboard
    for (const auto &keyMap: keyboard.mapping) {
        if (getKeyState(keyMap.value)) {
            players[0].buttons |= keyMap.button;
        }
    }

    /// process touch
    players[0].touch = getTouch();
    if (players[0].touch != Vector2f()) {
        players[0].buttons |= Input::Button::Touch;
    }

    /// process auto repeat
    if (!m_repeat || players[0].buttons & Input::Button::Quit) {
        m_stateOld = players[0].buttons;
        return players;
    }

    if (elapsed >= m_repeatDelay) {
        m_repeatClock->restart();
        m_stateOld = players[0].buttons;
        return players;
    } else {
        unsigned int diff = m_stateOld ^ players[0].buttons;
        m_stateOld = players[0].buttons;
        if (diff > 0) {
            m_repeatClock->restart();
        } else {
            players[0].buttons = Button::Delay;
        }
    }

    return players;
}

void Input::setRepeatDelay(int ms) {
    m_repeatDelay = ms;
    m_repeat = ms > 0;
}

int Input::getRepeatDelay() {
    return m_repeatDelay;
}

int Input::clear(int player) {
    while (true) {
        Player p = update()[player];
        if (!p.enabled || p.buttons == 0 || p.buttons & Input::Button::Quit) {
            break;
        }
    }

    return 0;
}

void Input::setJoystickMapping(int player, const std::vector<ButtonMapping> &mapping,
                               const Vector2i &leftAxis, const Vector2i &rightAxis, int dz) {
    if (player < PLAYER_MAX) {
        players[player].mapping = mapping;
        players[player].lx.id = leftAxis.x;
        players[player].ly.id = leftAxis.y;
        players[player].rx.id = rightAxis.x;
        players[player].ry.id = rightAxis.y;
        players[player].dz = dz;
    }
}

void Input::setRotation(const Input::Rotation &rotation) {
    m_rotation = rotation;
}

Input::Rotation Input::getRotation() {
    return m_rotation;
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

std::vector<Input::ButtonMapping> Input::getKeyboardMapping() {
    return keyboard.mapping;
}

std::vector<Input::ButtonMapping> Input::getKeyboardMappingDefault() {
    return keyboard.mapping_default;
}

unsigned int Input::getButtonRotation(unsigned int button) {
    if (m_rotation == R90) {
        if (button & Button::Up) {
            return Button::Left;
        } else if (button & Button::Right) {
            return Button::Up;
        } else if (button & Button::Down) {
            return Button::Right;
        } else if (button & Button::Left) {
            return Button::Down;
        } else if (button & Button::Y) {
            return Button::X;
        } else if (button & Button::B) {
            return Button::Y;
        } else if (button & Button::A) {
            return Button::B;
        } else if (button & Button::X) {
            return Button::A;
        }
    } else if (m_rotation == R180) {
        if (button & Button::Up) {
            return Button::Down;
        } else if (button & Button::Right) {
            return Button::Left;
        } else if (button & Button::Down) {
            return Button::Up;
        } else if (button & Button::Left) {
            return Button::Right;
        } else if (button & Button::Y) {
            return Button::A;
        } else if (button & Button::B) {
            return Button::X;
        } else if (button & Button::A) {
            return Button::Y;
        } else if (button & Button::X) {
            return Button::B;
        }
    } else if (m_rotation == R270) {
        if (button & Button::Up) {
            return Button::Right;
        } else if (button & Button::Right) {
            return Button::Down;
        } else if (button & Button::Down) {
            return Button::Left;
        } else if (button & Button::Left) {
            return Button::Up;
        } else if (button & Button::Y) {
            return Button::B;
        } else if (button & Button::B) {
            return Button::A;
        } else if (button & Button::A) {
            return Button::X;
        } else if (button & Button::X) {
            return Button::Y;
        }
    }

    return button;
}

Input::~Input() {
    delete (m_repeatClock);
}

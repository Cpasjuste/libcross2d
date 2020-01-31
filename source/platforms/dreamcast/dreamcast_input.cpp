//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

static int key_id[KEY_COUNT]{
        Input::Key::Up,
        Input::Key::Down,
        Input::Key::Left,
        Input::Key::Right,
        Input::Key::Select,
        Input::Key::Start,
        Input::Key::Fire1,
        Input::Key::Fire2,
        Input::Key::Fire3,
        Input::Key::Fire4,
        Input::Key::Fire5,
        Input::Key::Fire6
};

DCInput::DCInput() : Input() {

    for (int i = 0; i < PLAYER_MAX; i++) {
        InputData *data = new InputData();
        players[i].data = data;
        data->cont = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
        if (data->cont != nullptr) {
            players[i].id = i;
            players[i].enabled = true;
            printf("Joystick[%i] enabled...\n", i);
        }
    }

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

DCInput::~DCInput() {

    for (int i = 0; i < PLAYER_MAX; i++) {
        players[i].enabled = false;
        delete ((InputData *) players[i].data);
    }
}

int DCInput::waitButton(int player) {
    // TODO
    return -1;
}

Input::Player *DCInput::update(int rotate) {

    for (auto &player : players) {

        if (!player.enabled) {
            continue;
        }

        // reset keys
        player.keys = 0;
        InputData *data = (InputData *) player.data;
        data->state = (cont_state_t *) maple_dev_status(data->cont);
        if (data->state == nullptr) {
            continue;
        }

        // sticks
        process_axis(player, rotate);

        // buttons
        process_buttons(player, rotate);
    }

    // mandatory to handle repeat delay if needed
    return Input::update(rotate);
}

void DCInput::process_axis(Input::Player &player, int rotate) {

    if (!player.enabled || (player.data == nullptr)) {
        return;
    }

    float analogX, analogY;
    auto deadZone = (float) player.dead_zone;
    float scalingFactor, magnitude;
    bool up = false, down = false, left = false, right = false;
    Axis *currentStickXAxis = nullptr;
    Axis *currentStickYAxis = nullptr;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones

    for (int i = 0; i <= 1; i++) {

        if (i == 0) {
            // left stick
            currentStickXAxis = &(player.lx);
            currentStickYAxis = &(player.ly);
        } else {
            // right stick
            currentStickXAxis = &(player.rx);
            currentStickYAxis = &(player.ry);
        }

        InputData *data = (InputData *) player.data;
        analogX = (float) (i == 0 ? data->state->joyx : data->state->joy2x);
        analogY = (float) (i == 0 ? data->state->joyy : data->state->joy2y);

        //radial and scaled deadzone
        //http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html

        if ((magnitude = std::sqrt(analogX * analogX + analogY * analogY)) >= deadZone) {

            // analog control
            scalingFactor = 127.0f / magnitude * (magnitude - deadZone) / (128.0f - deadZone);
            currentStickXAxis->value = (short) (analogX * scalingFactor);
            currentStickYAxis->value = (short) (analogY * scalingFactor);

            // symmetric angular zones for all eight digital directions
            analogY = -analogY;
            if (analogY > 0 && analogX > 0) {
                // upper right quadrant
                if (analogY > slope * analogX)
                    up = true;
                if (analogX > slope * analogY)
                    right = true;
            } else if (analogY > 0 && analogX <= 0) {
                // upper left quadrant
                if (analogY > slope * (-analogX))
                    up = true;
                if ((-analogX) > slope * analogY)
                    left = true;
            } else if (analogY <= 0 && analogX > 0) {
                // lower right quadrant
                if ((-analogY) > slope * analogX)
                    down = true;
                if (analogX > slope * (-analogY))
                    right = true;
            } else if (analogY <= 0 && analogX <= 0) {
                // lower left quadrant
                if ((-analogY) > slope * (-analogX))
                    down = true;
                if ((-analogX) > slope * (-analogY))
                    left = true;
            }

            if (right)
                player.keys |= (rotate == 1) ? Input::Key::Down :
                               (rotate == 3) ? Input::Key::Up : Input::Key::Right;
            if (left)
                player.keys |= (rotate == 1) ? Input::Key::Up :
                               (rotate == 3) ? Input::Key::Down : Input::Key::Left;
            if (up)
                player.keys |= (rotate == 1) ? Input::Key::Right :
                               (rotate == 3) ? Input::Key::Left : Input::Key::Up;
            if (down)
                player.keys |= (rotate == 1) ? Input::Key::Left :
                               (rotate == 3) ? Input::Key::Right : Input::Key::Down;
        } else {
            currentStickXAxis->value = 0;
            currentStickYAxis->value = 0;
        } // end if (magnitude >= deadZone)
    } // end for
}

void DCInput::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || player.data == nullptr) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];
        InputData *data = (InputData *) player.data;
        uint32 buttons = data->state->buttons;

        if (buttons & mapping) {
            if (rotate && key_id[i] == Input::Key::Up) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Right;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Left;
                }
            } else if (rotate && key_id[i] == Input::Key::Down) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Left;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Right;
                }
            } else if (rotate && key_id[i] == Input::Key::Left) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Up;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Down;
                }
            } else if (rotate && key_id[i] == Input::Key::Right) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Down;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Up;
                }
            } else {
                player.keys |= key_id[i];
            }
        }

        if (key_id[i] == Input::Key::Fire5 && data->state->ltrig > 0) {
            player.keys |= Input::Key::Fire5;
        }

        if (key_id[i] == Input::Key::Fire6 && data->state->rtrig > 0) {
            player.keys |= Input::Key::Fire6;
        }
    }
}

void DCInput::setJoystickMapping(int player, const int *mapping, int deadzone) {
    Input::setJoystickMapping(player, mapping, 40);
}

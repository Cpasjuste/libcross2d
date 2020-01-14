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

    int joystick_count = 4;
    if (joystick_count > PLAYER_MAX) {
        joystick_count = PLAYER_MAX;
    }
    printf("%d Joystick(s) Found\n", joystick_count);

    if (joystick_count > 0) {
        for (int i = 0; i < joystick_count; i++) {
            printf("Joystick: %i\n", i);
            InputData *data = new InputData();
            data->cont = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
            data->state = (cont_state_t *) maple_dev_status(data->cont);
            players[i].data = data;
            players[i].id = i;
            players[i].enabled = true;
        }
    } else {
        // allow keyboard mapping to player1
        players[0].enabled = true;
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
        player.keys = 0;
    }

    for (auto &player : players) {

        if (!player.enabled) {
            continue;
        }

        InputData *data = (InputData *) player.data;
        data->state = (cont_state_t *) maple_dev_status(data->cont);

        // hat
        process_hat(player, rotate);

        // sticks
        process_axis(player, rotate);

        // buttons
        process_buttons(player, rotate);
    }

    // keyboard
    process_keyboard(players[0], rotate);

    // mandatory to handle repeat delay if needed
    return Input::update(rotate);
}

void DCInput::process_axis(Input::Player &player, int rotate) {

    /*
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
        analogX = (float) (SDL_JoystickGetAxis((SDL_Joystick *) player.data, currentStickXAxis->id));
        analogY = (float) (SDL_JoystickGetAxis((SDL_Joystick *) player.data, currentStickYAxis->id));

        //radial and scaled deadzone
        //http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html

        if ((magnitude = std::sqrt(analogX * analogX + analogY * analogY)) >= deadZone) {

            // analog control
            scalingFactor = 32767.0f / magnitude * (magnitude - deadZone) / (32769.0f - deadZone);
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
    */
}

void DCInput::process_hat(Input::Player &player, int rotate) {

    /*
    if (!player.enabled || !player.data) {
        return;
    }

    int value = SDL_JoystickGetHat((SDL_Joystick *) player.data, 0);

    if (value == SDL_HAT_UP
        || value == SDL_HAT_LEFTUP
        || value == SDL_HAT_RIGHTUP) {
        player.keys |= (rotate == 1) ?
                       Input::Key::Right : (rotate == 3) ? Input::Key::Left
                                                         : Input::Key::Up;
    }
    if (value == SDL_HAT_DOWN
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_RIGHTDOWN) {
        player.keys |= (rotate == 1) ?
                       Input::Key::Left : (rotate == 3) ? Input::Key::Right
                                                        : Input::Key::Down;
    }
    if (value == SDL_HAT_LEFT
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_LEFTUP) {
        player.keys |= (rotate == 1) ?
                       Input::Key::Up : (rotate == 3) ? Input::Key::Down
                                                      : Input::Key::Left;
    }
    if (value == SDL_HAT_RIGHT
        || value == SDL_HAT_RIGHTDOWN
        || value == SDL_HAT_RIGHTUP) {
        player.keys |= (rotate == 1) ?
                       Input::Key::Down : (rotate == 3) ? Input::Key::Up
                                                        : Input::Key::Right;
    }
    */
}

void DCInput::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || player.data == nullptr) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];
        InputData *data = (InputData *) player.data;
        if (data->state->buttons & mapping) {
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
    }
}

void DCInput::process_keyboard(Input::Player &player, int rotate) {
#ifndef NO_KEYBOARD
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < KEY_COUNT; i++) {
        if (keys[keyboard.mapping[i]]) {
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
    }
#endif
}

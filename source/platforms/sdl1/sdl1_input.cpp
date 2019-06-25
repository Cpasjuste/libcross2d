//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"

#if SDL_VERSION_ATLEAST(1, 3, 0)
#include <SDL/SDL_keyboard.h>
#endif

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

SDL1Input::SDL1Input() : Input() {

    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        printf("SDL1Input: SDL_INIT_JOYSTICK\n");
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    int joystick_count = SDL_NumJoysticks();
    if (joystick_count > PLAYER_MAX) {
        joystick_count = PLAYER_MAX;
    }
    printf("%d Joystick(s) Found\n", joystick_count);

    if (joystick_count > 0) {
        for (int i = 0; i < joystick_count; i++) {
            printf("Joystick: %i\n", i);
            players[i].data = SDL_JoystickOpen(i);
            players[i].id = i;
            players[i].enabled = true;
            printf("Name: %s\n", SDL_JoystickName(i));
            printf("Hats %d\n", SDL_JoystickNumHats((SDL_Joystick *) players[i].data));
            printf("Buttons %d\n", SDL_JoystickNumButtons((SDL_Joystick *) players[i].data));
            printf("Axis %d\n", SDL_JoystickNumAxes((SDL_Joystick *) players[i].data));
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

SDL1Input::~SDL1Input() {

    for (int i = 0; i < PLAYER_MAX; i++) {
        players[i].enabled = false;
    }

    if (SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
}

int SDL1Input::waitButton(int player) {

    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            return event.jbutton.button;
        } else if (event.type == SDL_KEYDOWN) {
            return event.key.keysym.scancode;
        }
    }
    return -1;
}

Input::Player *SDL1Input::update(int rotate) {

    for (auto &player : players) {
        player.keys = 0;
    }

    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            players[0].keys |= EV_QUIT;
            return players;
        }
    }

    SDL_JoystickUpdate(); // ensure all joysticks are up-to-date to remove lag

    for (auto &player : players) {

        if (!player.enabled) {
            continue;
        }

        // hat
        process_hat(player, rotate);

        // sticks
        process_axis(player, rotate);

        // buttons
        process_buttons(player, rotate);

        // mouse (touch)
        // process_touch(player);
    }

    // keyboard
    process_keyboard(players[0], rotate);

    // mandatory to handle repeat delay if needed
    return Input::update(rotate);
}

void SDL1Input::process_axis(Input::Player &player, int rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    float analogX, analogY;
    float deadZone = (float) player.dead_zone;
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
}

void SDL1Input::process_hat(Input::Player &player, int rotate) {

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
}

void SDL1Input::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];

#ifdef __PSP2__
        // rotate buttons on ps vita to play in portrait mode
        if (rotate == 1) {
            switch (mapping) {
                case KEY_JOY_FIRE1_DEFAULT: // PSP2_CROSS (SDL-Vita)
                    mapping = KEY_JOY_FIRE2_DEFAULT; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case KEY_JOY_FIRE3_DEFAULT: // PSP2_SQUARE (SDL-Vita)
                    mapping = KEY_JOY_FIRE1_DEFAULT; // PSP2_CROSS (SDL-Vita)
                    break;
                case KEY_JOY_FIRE4_DEFAULT: // PSP2_TRIANGLE (SDL-Vita)
                    mapping = KEY_JOY_FIRE3_DEFAULT; // PSP2_SQUARE (SDL-Vita)
                    break;
                case KEY_JOY_FIRE2_DEFAULT: // PSP2_CIRCLE (SDL-Vita)
                    mapping = KEY_JOY_FIRE4_DEFAULT; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                default:
                    break;
            }
        } else if (rotate == 3) {
            switch (mapping) {
                case KEY_JOY_FIRE1_DEFAULT: // PSP2_CROSS (SDL-Vita)
                    mapping = KEY_JOY_FIRE3_DEFAULT; // PSP2_SQUARE (SDL-Vita)
                    break;
                case KEY_JOY_FIRE3_DEFAULT: // PSP2_SQUARE (SDL-Vita)
                    mapping = KEY_JOY_FIRE4_DEFAULT; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                case KEY_JOY_FIRE4_DEFAULT: // PSP2_TRIANGLE (SDL-Vita)
                    mapping = KEY_JOY_FIRE2_DEFAULT; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case KEY_JOY_FIRE2_DEFAULT: // PSP2_CIRCLE (SDL-Vita)
                    mapping = KEY_JOY_FIRE1_DEFAULT; // PSP2_CROSS (SDL-Vita)
                    break;
                default:
                    break;
            }
        }
#endif

        if (SDL_JoystickGetButton((SDL_Joystick *) player.data, mapping)) {
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

void SDL1Input::process_keyboard(Input::Player &player, int rotate) {

#if SDL_VERSION_ATLEAST(1, 3, 0)
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
#else
    const Uint8 *keys = SDL_GetKeyState(nullptr);
#endif

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
}

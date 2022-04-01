//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

SDL2Input::SDL2Input() : Input() {
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0) {
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    }

    int joystick_count = SDL_NumJoysticks();
    if (joystick_count > PLAYER_MAX) {
        joystick_count = PLAYER_MAX;
    }

    printf("SDL2Input: %i Joystick(s) Found\n", joystick_count);

    if (joystick_count > 0) {
        for (int i = 0; i < joystick_count; i++) {
            if (!SDL_IsGameController(i)) {
                printf("SDL2Input: joystick %i is not a game controller, skipping!", i);
                continue;
            }

            SDL_GameController *pad = SDL_GameControllerOpen(i);
            char *mapping = SDL_GameControllerMapping(pad);
            printf("SDL2Input: GameController detected: %s, mapping: %s\n",
                   SDL_GameControllerName(pad), mapping);
            SDL_free(mapping);

            players[i].data = pad;
            players[i].id = i;
            players[i].enabled = true;
        }
    } else {
        // allow keyboard mapping to player1
        players[0].enabled = true;
    }
}

SDL2Input::~SDL2Input() {
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER)) {
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    }
}

bool SDL2Input::waitButton(unsigned int *key, int player) {
    SDL_Event event = {};

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (key) {
                *key = event.cbutton.button;
            }
            return true;
        } else if (event.type == SDL_KEYDOWN) {
            if (key) {
                *key = event.key.keysym.scancode;
            }
            return true;
        }
    }

    return false;
}

Input::Player *SDL2Input::update(int rotate) {
    for (auto &player: players) {
        player.buttons = 0;
    }

    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            players[0].buttons |= Input::Button::Quit;
            return players;
        }
    }

    for (auto &player: players) {
        if (!player.enabled) {
            continue;
        }

        // sticks
        process_axis(player, rotate);

        // buttons
        process_buttons(player, rotate);

        // mouse (touch)
        process_touch(player);
    }

    // keyboard
    process_keyboard(players[0], rotate);

    // mandatory to handle repeat delay if needed
    return Input::update(rotate);
}

void SDL2Input::process_axis(Input::Player &player, int rotate) {
    if (!player.enabled || !player.data) {
        return;
    }

    float analogX, analogY;
    auto deadZone = (float) player.dz;
    float scalingFactor, magnitude;
    bool up = false, down = false, left = false, right = false;
    Axis *currentStickXAxis;
    Axis *currentStickYAxis;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones

    auto pad = (SDL_GameController *) player.data;

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

        analogX = (float) (SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) currentStickXAxis->id));
        analogY = (float) (SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) currentStickYAxis->id));

        // radial and scaled deadzone
        // http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html

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
                player.buttons |= (rotate == 1) ? Input::Button::Down :
                                  (rotate == 3) ? Input::Button::Up : Input::Button::Right;
            if (left)
                player.buttons |= (rotate == 1) ? Input::Button::Up :
                                  (rotate == 3) ? Input::Button::Down : Input::Button::Left;
            if (up)
                player.buttons |= (rotate == 1) ? Input::Button::Right :
                                  (rotate == 3) ? Input::Button::Left : Input::Button::Up;
            if (down)
                player.buttons |= (rotate == 1) ? Input::Button::Left :
                                  (rotate == 3) ? Input::Button::Right : Input::Button::Down;
        } else {
            currentStickXAxis->value = 0;
            currentStickYAxis->value = 0;
        } // end if (magnitude >= deadZone)
    } // end for
}

void SDL2Input::process_buttons(Input::Player &player, int rotate) {
    if (!player.enabled || !player.data) {
        return;
    }

    auto pad = (SDL_GameController *) player.data;

    for (const auto &buttonMap: player.mapping) {
        int button = buttonMap.value;
#ifdef __PSP2__
        // rotate buttons on ps vita to play in portrait mode
        if (rotate == 1) {
            switch (button) {
                case KEY_JOY_A_DEFAULT: // PSP2_CROSS (SDL-Vita)
                    button = KEY_JOY_B_DEFAULT; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case KEY_JOY_X_DEFAULT: // PSP2_SQUARE (SDL-Vita)
                    button = KEY_JOY_A_DEFAULT; // PSP2_CROSS (SDL-Vita)
                    break;
                case KEY_JOY_Y_DEFAULT: // PSP2_TRIANGLE (SDL-Vita)
                    button = KEY_JOY_X_DEFAULT; // PSP2_SQUARE (SDL-Vita)
                    break;
                case KEY_JOY_B_DEFAULT: // PSP2_CIRCLE (SDL-Vita)
                    button = KEY_JOY_Y_DEFAULT; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                default:
                    break;
            }
        } else if (rotate == 3) {
            switch (button) {
                case KEY_JOY_A_DEFAULT: // PSP2_CROSS (SDL-Vita)
                    button = KEY_JOY_X_DEFAULT; // PSP2_SQUARE (SDL-Vita)
                    break;
                case KEY_JOY_X_DEFAULT: // PSP2_SQUARE (SDL-Vita)
                    button = KEY_JOY_Y_DEFAULT; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                case KEY_JOY_Y_DEFAULT: // PSP2_TRIANGLE (SDL-Vita)
                    button = KEY_JOY_B_DEFAULT; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case KEY_JOY_B_DEFAULT: // PSP2_CIRCLE (SDL-Vita)
                    button = KEY_JOY_A_DEFAULT; // PSP2_CROSS (SDL-Vita)
                    break;
                default:
                    break;
            }
        }
#endif

        // process shoulder buttons if any
        if (button == ((int) SDL_CONTROLLER_AXIS_TRIGGERLEFT + 100)
            || button == ((int) SDL_CONTROLLER_AXIS_TRIGGERRIGHT + 100)) {
            bool pressed = SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) (button - 100)) > player.dz;
            if (pressed > 0) {
                player.buttons |= buttonMap.button;
            }
            continue;
        }

        if (SDL_GameControllerGetButton(pad, (SDL_GameControllerButton) button)) {
            if (rotate && buttonMap.button == Input::Button::Up) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Right;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Left;
                }
            } else if (rotate && buttonMap.button == Input::Button::Down) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Left;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Right;
                }
            } else if (rotate && buttonMap.button == Input::Button::Left) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Up;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Down;
                }
            } else if (rotate && buttonMap.button == Input::Button::Right) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Down;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Up;
                }
            } else {
                player.buttons |= buttonMap.button;
            }
        }
    }
}

void SDL2Input::process_keyboard(Input::Player &player, int rotate) {
#ifndef NO_KEYBOARD
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);

    for (const auto &buttonMap: player.mapping) {
        if (keys[buttonMap.value]) {
            if (rotate && buttonMap.button == Input::Button::Up) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Right;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Left;
                }
            } else if (rotate && buttonMap.button == Input::Button::Down) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Left;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Right;
                }
            } else if (rotate && buttonMap.button == Input::Button::Left) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Up;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Down;
                }
            } else if (rotate && buttonMap.button == Input::Button::Right) {
                if (rotate == 1) {
                    player.buttons |= Input::Button::Down;
                } else if (rotate == 3) {
                    player.buttons |= Input::Button::Up;
                }
            } else {
                player.buttons |= buttonMap.button;
            }
        }
    }
#endif
}

void SDL2Input::process_touch(Input::Player &player) {
    int x, y;

    player.touch = Vector2f(0, 0);

    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player.touch.x = (float) x;
        player.touch.y = (float) y;
        player.buttons |= Input::Button::Touch;
    }
}

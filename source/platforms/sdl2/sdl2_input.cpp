//
// Created by cpasjuste on 11/01/17.
//

#include <cmath>
#include "skeleton/renderer.h"
#include "platforms/sdl2/sdl2_input.h"

using namespace c2d;

static int key_id[KEY_COUNT]{
        Input::Key::KEY_UP,
        Input::Key::KEY_DOWN,
        Input::Key::KEY_LEFT,
        Input::Key::KEY_RIGHT,
        Input::Key::KEY_COIN,
        Input::Key::KEY_START,
        Input::Key::KEY_FIRE1,
        Input::Key::KEY_FIRE2,
        Input::Key::KEY_FIRE3,
        Input::Key::KEY_FIRE4,
        Input::Key::KEY_FIRE5,
        Input::Key::KEY_FIRE6,
        Input::Key::KEY_MENU1,
        Input::Key::KEY_MENU2
};

SDL2Input::SDL2Input(Renderer *r) : Input(r) {

    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        printf("SDL2Input: SDL_INIT_JOYSTICK\n");
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    int joystick_count = SDL_NumJoysticks();
    if (joystick_count > 4) {
        joystick_count = 4;
    }
    printf("%d Joystick(s) Found\n", joystick_count);

    if (joystick_count > 0) {
        for (int i = 0; i < joystick_count; i++) {
            printf("Joystick: %i\n", i);
            players[i].data = SDL_JoystickOpen(i);
            players[i].id = i;
            players[i].enabled = true;
            printf("Name: %s\n", SDL_JoystickName((SDL_Joystick *) players[i].data));
            printf("Hats %d\n", SDL_JoystickNumHats((SDL_Joystick *) players[i].data));
            printf("Buttons %d\n", SDL_JoystickNumButtons((SDL_Joystick *) players[i].data));
            printf("Axis %d\n", SDL_JoystickNumAxes((SDL_Joystick *) players[i].data));
        }
    } else {
        // allow keyboard mapping to player1
        players[0].enabled = true;
    }

    for (int i = 0; i < PLAYER_COUNT; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

SDL2Input::~SDL2Input() {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].enabled = false;
    }

    if (SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
}

int SDL2Input::getButton(int player) {
    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            return event.jbutton.button;
        }
    }
    return -1;
}

Input::Player *SDL2Input::update(int rotate) {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].state = 0;
    }

    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            players[0].state |= EV_QUIT;
            return players;
        }
    }

    SDL_JoystickUpdate(); // ensure all joysticks are up-to-date to remove lag

    for (int i = 0; i < PLAYER_COUNT; i++) {

        if (!players[i].enabled) {
            continue;
        }

        // hat
        process_hat(players[i], rotate);

        // sticks
        process_axis(players[i], rotate);

        // buttons
        process_buttons(players[i], rotate);
    }

    // keyboard
    process_keyboard(players[0], rotate);

    return players;
}

void SDL2Input::process_axis(Input::Player &player, int rotate) {

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
                player.state |= (rotate == 1) ? Input::Key::KEY_DOWN :
                                (rotate == 3) ? Input::Key::KEY_UP : Input::Key::KEY_RIGHT;
            if (left)
                player.state |= (rotate == 1) ? Input::Key::KEY_UP :
                                (rotate == 3) ? Input::Key::KEY_DOWN : Input::Key::KEY_LEFT;
            if (up)
                player.state |= (rotate == 1) ? Input::Key::KEY_RIGHT :
                                (rotate == 3) ? Input::Key::KEY_LEFT : Input::Key::KEY_UP;
            if (down)
                player.state |= (rotate == 1) ? Input::Key::KEY_LEFT :
                                (rotate == 3) ? Input::Key::KEY_RIGHT : Input::Key::KEY_DOWN;
        } else {
            currentStickXAxis->value = 0;
            currentStickYAxis->value = 0;
        } // end if (magnitude >= deadZone)
    } // end for
}

void SDL2Input::process_hat(Input::Player &player, int rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    int value = SDL_JoystickGetHat((SDL_Joystick *) player.data, 0);

    if (value == SDL_HAT_UP
        || value == SDL_HAT_LEFTUP
        || value == SDL_HAT_RIGHTUP) {
        player.state |= (rotate == 1) ? Input::Key::KEY_RIGHT : (rotate == 3) ? Input::Key::KEY_LEFT
                                                                              : Input::Key::KEY_UP;
    }
    if (value == SDL_HAT_DOWN
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_RIGHTDOWN) {
        player.state |= (rotate == 1) ? Input::Key::KEY_LEFT : (rotate == 3) ? Input::Key::KEY_RIGHT
                                                                             : Input::Key::KEY_DOWN;
    }
    if (value == SDL_HAT_LEFT
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_LEFTUP) {
        player.state |= (rotate == 1) ? Input::Key::KEY_UP : (rotate == 3) ? Input::Key::KEY_DOWN
                                                                           : Input::Key::KEY_LEFT;
    }
    if (value == SDL_HAT_RIGHT
        || value == SDL_HAT_RIGHTDOWN
        || value == SDL_HAT_RIGHTUP) {
        player.state |= (rotate == 1) ? Input::Key::KEY_DOWN : (rotate == 3) ? Input::Key::KEY_UP
                                                                             : Input::Key::KEY_RIGHT;
    }
}

void SDL2Input::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];

#ifdef __PSP2__
        // rotate buttons on ps vita to play in portrait mode
        if (rotate == 1) {
            switch (mapping) {
                case 2: // PSP2_CROSS (SDL-Vita)
                    mapping = 1; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case 3: // PSP2_SQUARE (SDL-Vita)
                    mapping = 2; // PSP2_CROSS (SDL-Vita)
                    break;
                case 0: // PSP2_TRIANGLE (SDL-Vita)
                    mapping = 3; // PSP2_SQUARE (SDL-Vita)
                    break;
                case 1: // PSP2_CIRCLE (SDL-Vita)
                    mapping = 0; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                default:
                    break;
            }
        } else if (rotate == 3) {
            switch (mapping) {
                case 2: // PSP2_CROSS (SDL-Vita)
                    mapping = 3; // PSP2_SQUARE (SDL-Vita)
                    break;
                case 3: // PSP2_SQUARE (SDL-Vita)
                    mapping = 0; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                case 0: // PSP2_TRIANGLE (SDL-Vita)
                    mapping = 1; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case 1: // PSP2_CIRCLE (SDL-Vita)
                    mapping = 2; // PSP2_CROSS (SDL-Vita)
                    break;
                default:
                    break;
            }
        }
#endif

        if (SDL_JoystickGetButton((SDL_Joystick *) player.data, mapping)) {
            if (rotate && key_id[i] == Input::Key::KEY_UP) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_RIGHT;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_LEFT;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_DOWN) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_LEFT;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_RIGHT;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_LEFT) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_UP;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_DOWN;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_RIGHT) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_DOWN;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_UP;
                }
            } else {
                player.state |= key_id[i];
            }
        }
    }
}

void SDL2Input::process_keyboard(Input::Player &player, int rotate) {

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < KEY_COUNT; i++) {
        if (keys[keyboard.mapping[i]]) {
            if (rotate && key_id[i] == Input::Key::KEY_UP) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_RIGHT;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_LEFT;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_DOWN) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_LEFT;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_RIGHT;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_LEFT) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_UP;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_DOWN;
                }
            } else if (rotate && key_id[i] == Input::Key::KEY_RIGHT) {
                if (rotate == 1) {
                    player.state |= Input::Key::KEY_DOWN;
                } else if (rotate == 3) {
                    player.state |= Input::Key::KEY_UP;
                }
            } else {
                player.state |= key_id[i];
            }
        }
    }
}

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

Input::Player *SDL2Input::update() {
    Player *players = Input::update();

    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            players[0].buttons |= Input::Button::Quit;
            return players;
        }
    }

    return players;
}

Vector2f SDL2Input::getAxisState(const Player &player, int xAxis, int yAxis) {
    auto pad = (SDL_GameController *) player.data;

    return {
            (float) SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) xAxis),
            (float) SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) yAxis)
    };
}

int SDL2Input::getButtonState(const Player &player, int button) {
    auto pad = (SDL_GameController *) player.data;

    if (button == ((int) SDL_CONTROLLER_AXIS_TRIGGERLEFT + 100)
        || button == ((int) SDL_CONTROLLER_AXIS_TRIGGERRIGHT + 100)) {
        return SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis) (button - 100)) > player.dz;
    }

    return SDL_GameControllerGetButton(pad, (SDL_GameControllerButton) button) > 0;
}

int SDL2Input::getKeyState(int key) {
#ifndef NO_KEYBOARD
    return SDL_GetKeyboardState(nullptr)[key] > 0;
#endif
    return 0;
}

Vector2f SDL2Input::getTouch() {
    int x, y;

    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        return {(float) x, (float) y};
    }

    return {};
}

bool SDL2Input::waitButton(unsigned int *key, int player) {
    SDL_Event event = {};

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (key) {
                *key = event.cbutton.button;
            }
            return true;
        } else if (event.type == SDL_CONTROLLERAXISMOTION) {
            if ((event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT ||
                 event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
                && (event.caxis.value < -16000 || event.caxis.value > 16000)) {
                if (key) {
                    *key = event.caxis.axis + 100;
                }
                return true;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (key) {
                *key = event.key.keysym.scancode;
            }
            return true;
        }
    }

    return false;
}

SDL2Input::~SDL2Input() {
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER)) {
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    }
}

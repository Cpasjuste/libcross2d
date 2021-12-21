//
// Created by cpasjuste on 11/01/17.
//

#include <libScePad.h>
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

PS4Input::PS4Input() : Input() {

    // be sure modules are loaded
    auto renderer = (PS4Renderer *) c2d_renderer;
    if (!renderer->loadModules()) {
        return;
    }

    int ret;
    int param = 700;
    ret = sceUserServiceInitialize(&param);
    printf("PS4Input: sceUserServiceInitialize = 0x%08x\n", ret);
    ret = scePadInit();
    printf("PS4Input: scePadInit = 0x%08x\n", ret);

    // TODO: handle multiple gamepads
    SceUserServiceUserId userId;
    sceUserServiceGetInitialUser(&userId);
    players[0].id = scePadOpen(userId, 0, 0, nullptr);
    players[0].data = (ScePadData *) malloc(sizeof(ScePadData));
    memset(players[0].data, 0, sizeof(ScePadData));
    players[0].enabled = true;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

PS4Input::~PS4Input() {

    // TODO: handle multiple gamepads
    for (int i = 0; i < PLAYER_MAX; i++) {
        players[i].enabled = false;
    }

    scePadClose(players[0].id);
    if (players[0].data) free(players[0].data);
}

bool PS4Input::waitKey(unsigned int *key, int player) {

    // TODO
    return true;

    /*
    SDL_Event event = {};
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            if (key != nullptr) {
                *key = event.jbutton.button;
            }
            return true;
        } else if (event.type == SDL_KEYDOWN) {
            if (key != nullptr) {
                *key = event.key.keysym.scancode;
            }
            return true;
        }
    }

    return false;
    */
}

Input::Player *PS4Input::update(int rotate) {

    for (auto &player: players) {

        player.keys = 0;

        if (!player.enabled) {
            continue;
        }

        scePadReadState(player.id, (ScePadData *) player.data);

        // sticks
        process_axis(player, rotate);

        // buttons
        process_buttons(player, rotate);
    }

    // mandatory to handle repeat delay if needed
    return Input::update(rotate);
}

void PS4Input::process_axis(Input::Player &player, int rotate) {
    // TODO
#if 0
    if (!player.enabled || player.id < 0) {
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
#endif
}

void PS4Input::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || player.data == nullptr) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];
        auto *padData = (ScePadData *) player.data;

        if (padData->buttons & mapping) {
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

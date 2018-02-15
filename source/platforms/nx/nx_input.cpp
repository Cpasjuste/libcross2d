//
// Created by cpasjuste on 11/01/17.
//

#include <switch.h>
#include "c2d.h"

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

NXInput::NXInput(Renderer *r) : Input(r) {

    hidInitialize();

    players[0].enabled = true;

    for (int k = 0; k < KEY_COUNT; k++) {
        players[0].mapping[k] = 0;
    }
}

NXInput::~NXInput() {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].enabled = false;
    }

    hidExit();
}

int NXInput::getButton(int player) {

    // TODO:
    /*
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            return event.jbutton.button;
        }
    }
    */

    return -1;
}

Input::Player *NXInput::update(int rotate) {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].state = 0;
    }


    if (!appletMainLoop()) {
        players[0].state |= EV_QUIT;
        return players;
    }

    hidScanInput();

    process_buttons(players[0], rotate);

    // 3ds needs screen refresh/swap every frames ?
    if (players[0].state <= 0) {
        players[0].state = EV_REFRESH;
    }

    return players;
}

void NXInput::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled) {
        return;
    }

    u64 held = hidKeysHeld(CONTROLLER_P1_AUTO);
    //printf("hidKeysHeld: %i\n", held);

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];
        if (mapping < 0)
            mapping = 0;

        if (held & BIT(mapping)) {
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

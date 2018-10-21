//
// Created by cpasjuste on 11/01/17.
//

#include <3ds.h>
#include "cross2d/platforms/3ds/ctr_input.h"

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
        Input::Key::KEY_FIRE6
};

CTRInput::CTRInput() : Input() {

    players[0].enabled = true;

    for (auto &player : players) {
        for (int &key : player.mapping) {
            key = 0;
        }
    }
}

CTRInput::~CTRInput() {

    for (auto &player : players) {
        player.enabled = false;
    }
}

int CTRInput::waitButton(int player) {

    // TODO:
    return -1;
}

Input::Player *CTRInput::update(int rotate) {

    for (auto &player : players) {
        player.state = 0;
    }


    if (!aptMainLoop()) {
        players[0].state |= EV_QUIT;
        return players;
    }

    hidScanInput();
    circlePosition circle;
    hidCircleRead(&circle);

    process_buttons(players[0], rotate);

    return players;
}

void CTRInput::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled) {
        return;
    }

    u32 held = hidKeysHeld();

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

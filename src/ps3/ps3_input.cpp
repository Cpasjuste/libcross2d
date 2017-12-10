//
// Created by cpasjuste on 11/01/17.
//

#include <cmath>
#include <cstdio>

#include <C2D.h>
#include <io/pad.h>

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

PS3Input::PS3Input(Renderer *r) : Input(r) {

    ioPadInit(7);

    padInfo padinfo;
    ioPadGetInfo(&padinfo);

    int count = 0;
    for (int i = 0; i < MAX_PADS; i++) {

        if (count > 3) {
            break;
        }

        if (padinfo.status[i]) {
            //players[count].data = SDL_JoystickOpen(i);
            players[count].id = i;
            players[count].enabled = true;
            printf("Joystick %u:\r\nVendor ID: %hx\r\nProduct ID: %hx\r\nStatus: %hhu\r\n",
                   i, padinfo.vendor_id[i], padinfo.product_id[i], padinfo.status[i]);
            count++;
        }
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

PS3Input::~PS3Input() {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].enabled = false;
    }

    ioPadEnd();
}

int PS3Input::GetButton(int player) {

    padInfo padinfo;
    padData paddata;

    ioPadGetInfo(&padinfo);
    if (padinfo.status[players[player].id]) {
        ioPadGetData((u32) players[player].id, &paddata);
        for (int i = 0; i < MAX_PAD_CODES; i++) {
            if (paddata.button[i]) {
                printf("btn: %i\n", i);
                return i;
            }
        }
    }

    return -1;
}

Input::Player *PS3Input::Update(int rotate) {

    for (int i = 0; i < PLAYER_COUNT; i++) {

        if (!players[i].enabled) {
            continue;
        }

        players[i].state = 0;

        padInfo padinfo;
        ioPadGetInfo(&padinfo);
        if (!padinfo.status[players[i].id]) {
            return players;
        }

        padData paddata;
        ioPadGetData((u32) players[i].id, &paddata);
        if (paddata.len < 8) {
            return players;
        }

        players[i].data = &paddata;

        // sticks
        process_axis(players[i], rotate);

        // buttons
        process_buttons(players[i], rotate);

        players[i].data = 0;
    }

    return players;
}

void PS3Input::process_axis(Input::Player &player, int rotate) {

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

        // TODO:
        //analogX = (float) (SDL_JoystickGetAxis((SDL_Joystick *) player.data, currentStickXAxis->id));
        //analogY = (float) (SDL_JoystickGetAxis((SDL_Joystick *) player.data, currentStickYAxis->id));

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

void PS3Input::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    padData *pad = (padData *) player.data;

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];

        if (pad->button[mapping]) {
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

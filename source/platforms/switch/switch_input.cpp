//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"
#include <cross2d/platforms/switch/switch_input.h>

using namespace c2d;

SWITCHInput::SWITCHInput() : SDL2Input() {
    padConfigureInput(8, HidNpadStyleSet_NpadStandard);
    padInitializeAny(&pad);
}

Input::Player *SWITCHInput::update(int rotate) {
    Input::Player *players = SDL2Input::update(rotate);

    padUpdate(&pad);

    // handle joycon's states (@ rsn8887)
    static int previous_single_joycon_mode = -1;
    static int previous_handheld_mode = -1;
    handheld_mode = padIsHandheld(&pad);

    if ((single_joycon_mode != previous_single_joycon_mode) || (handheld_mode != previous_handheld_mode)) {
        previous_handheld_mode = handheld_mode;
        previous_single_joycon_mode = single_joycon_mode;
        if (!handheld_mode) {
            if (single_joycon_mode) {
                hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                for (int id = 0; id < 8; id++) {
                    hidSetNpadJoyAssignmentModeSingleByDefault((HidNpadIdType) id);
                }
            } else {
                // find all left/right single JoyCon pairs and join them together
                for (int id = 0; id < 8; id++) {
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType) id);
                }
                int lastRightId = 8;
                for (int id0 = 0; id0 < 8; id0++) {
                    if (hidGetNpadDeviceType((HidNpadIdType) id0) & HidDeviceTypeBits_JoyLeft) {
                        for (int id1 = lastRightId - 1; id1 >= 0; id1--) {
                            if (hidGetNpadDeviceType((HidNpadIdType) id1) & HidDeviceTypeBits_JoyRight) {
                                lastRightId = id1;
                                // prevent missing player numbers
                                if (id0 < id1) {
                                    hidMergeSingleJoyAsDualJoy((HidNpadIdType) id0, (HidNpadIdType) id1);
                                } else if (id0 > id1) {
                                    hidMergeSingleJoyAsDualJoy((HidNpadIdType) id1, (HidNpadIdType) id0);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if (single_joycon_mode && !handheld_mode) {
        // handle missing (+) / (-) buttons on single joycon mode
        for (int i = 0; i < PLAYER_MAX; i++) {
            auto joystick = (SDL_Joystick *) players[i].data;
            int index = (int)SDL_JoystickInstanceID(joystick);
            if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyLeft) {
                int map[] = { 18,16,17,19,11,6,15,12,14,13,24,25};
                for (int j = 0; j < 12; j++)
                {
                    players[i].mapping[j] = map[j];
                }
                if (SDL_JoystickGetButton(joystick, KEY_JOY_ZL_DEFAULT))
                    players[i].keys |= Input::Key::Select;
            }
            else if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyRight) {
                int map[] = { 20,22,23,21,10,7,2,0,3,1,26,27 };
                for (int j = 0; j < 12; j++)
                {
                    players[i].mapping[j] = map[j];
                }
                if (SDL_JoystickGetButton(joystick, KEY_JOY_ZR_DEFAULT))
                    players[i].keys |= Input::Key::Select;
            }
        }
    }
    return players;
}

void SWITCHInput::setSingleJoyconMode(bool enable) {
    single_joycon_mode = enable;
}

void SWITCHInput::process_axis(Input::Player& player, int rotate)
{
    if (!player.enabled || !player.data || (single_joycon_mode && !handheld_mode)) {
        return;
    }

    float analogX, analogY;
    auto deadZone = (float)player.dead_zone;
    float scalingFactor, magnitude;
    bool up = false, down = false, left = false, right = false;
    Axis* currentStickXAxis = nullptr;
    Axis* currentStickYAxis = nullptr;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones

    for (int i = 0; i <= 1; i++) {

        if (i == 0) {
            // left stick
            currentStickXAxis = &(player.lx);
            currentStickYAxis = &(player.ly);
        }
        else {
            // right stick
            currentStickXAxis = &(player.rx);
            currentStickYAxis = &(player.ry);
        }
        analogX = (float)(SDL_JoystickGetAxis((SDL_Joystick*)player.data, currentStickXAxis->id));
        analogY = (float)(SDL_JoystickGetAxis((SDL_Joystick*)player.data, currentStickYAxis->id));

        //radial and scaled deadzone
        //http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html

        if ((magnitude = std::sqrt(analogX * analogX + analogY * analogY)) >= deadZone) {

            // analog control
            scalingFactor = 32767.0f / magnitude * (magnitude - deadZone) / (32769.0f - deadZone);
            currentStickXAxis->value = (short)(analogX * scalingFactor);
            currentStickYAxis->value = (short)(analogY * scalingFactor);

            // symmetric angular zones for all eight digital directions
            analogY = -analogY;
            if (analogY > 0 && analogX > 0) {
                // upper right quadrant
                if (analogY > slope * analogX)
                    up = true;
                if (analogX > slope * analogY)
                    right = true;
            }
            else if (analogY > 0 && analogX <= 0) {
                // upper left quadrant
                if (analogY > slope * (-analogX))
                    up = true;
                if ((-analogX) > slope * analogY)
                    left = true;
            }
            else if (analogY <= 0 && analogX > 0) {
                // lower right quadrant
                if ((-analogY) > slope * analogX)
                    down = true;
                if (analogX > slope * (-analogY))
                    right = true;
            }
            else if (analogY <= 0 && analogX <= 0) {
                // lower left quadrant
                if ((-analogY) > slope * (-analogX))
                    down = true;
                if ((-analogX) > slope * (-analogY))
                    left = true;
            }

            if (right)
                player.keys |= Input::Key::Right;
            if (left)
                player.keys |= Input::Key::Left;
            if (up)
                player.keys |= Input::Key::Up;
            if (down)
                player.keys |= Input::Key::Down;
        }
        else {
            currentStickXAxis->value = 0;
            currentStickYAxis->value = 0;
        }
    }
}

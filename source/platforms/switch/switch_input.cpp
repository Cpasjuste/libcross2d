//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

SWITCHInput::SWITCHInput() : SDL2Input() {
}

Input::Player *SWITCHInput::update(int rotate) {

    Input::Player *players = SDL2Input::update(rotate);

    // handle joycon's states (@ rsn8887)
    static int previous_single_joycon_mode = -1;
    static int previous_handheld_mode = -1;

    padConfigureInput(8, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeAny(&pad);
    padUpdate(&pad);
    int handheld_mode = padIsHandheld(&pad);

    if ((single_joycon_mode != previous_single_joycon_mode) || (handheld_mode != previous_handheld_mode)) {
        previous_handheld_mode = handheld_mode;
        previous_single_joycon_mode = single_joycon_mode;
        if (!handheld_mode) {
            if (single_joycon_mode) {
                for (int id = 0; id < 8; id++) {
                    hidSetNpadJoyAssignmentModeSingleByDefault((HidNpadIdType) id);
                }
                hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                //padUpdate(&pad);
            } else {
                // find all left/right single JoyCon pairs and join them together
                for (int id = 0; id < 8; id++) {
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType) id);
                }
                int lastRightId = 8;
                for (int id0 = 0; id0 < 8; id0++) {
                    if (hidGetNpadDeviceType((HidNpadIdType) id0) & HidNpadButton_AnyLeft) {
                        for (int id1 = lastRightId - 1; id1 >= 0; id1--) {
                            if (hidGetNpadDeviceType((HidNpadIdType) id1) & HidNpadButton_AnyRight) {
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
            if (SDL_JoystickGetButton(joystick, KEY_JOY_LSTICK_DEFAULT)) {
                int index = (int) SDL_JoystickInstanceID(joystick);
                if (hidGetNpadDeviceType((HidNpadIdType) index) & HidNpadButton_AnyLeft) {
                    players[i].keys |= Input::Key::Start;
                } else if (hidGetNpadDeviceType((HidNpadIdType) index) & HidNpadButton_AnyRight) {
                    players[i].keys |= Input::Key::Select;
                }
            }
        }
    }
    return players;
}

void SWITCHInput::setSingleJoyconMode(bool enable) {
    single_joycon_mode = enable;
}

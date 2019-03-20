//
// Created by cpasjuste on 11/01/17.
//

#include <cross2d/platforms/switch/switch_input.h>

#include "cross2d/c2d.h"

using namespace c2d;

SWITCHInput::SWITCHInput() : SDL2Input() {
}

Input::Player *SWITCHInput::update(int rotate) {

    // handle joycon's states (@ rsn8887)
    if (!hidGetHandheldMode()) {
        if (single_joycon_mode) {
            for (int id = 0; id < 8; id++) {
                hidSetNpadJoyAssignmentModeSingleByDefault((HidControllerID) id);
            }
            hidSetNpadJoyHoldType(HidJoyHoldType_Horizontal);
            hidScanInput();
        } else {
            // find all left/right single JoyCon pairs and join them together
            for (int id = 0; id < 8; id++) {
                hidSetNpadJoyAssignmentModeDual((HidControllerID) id);
            }
            int lastRightId = 8;
            for (int id0 = 0; id0 < 8; id0++) {
                if (hidGetControllerType((HidControllerID) id0) & TYPE_JOYCON_LEFT) {
                    for (int id1 = lastRightId - 1; id1 >= 0; id1--) {
                        if (hidGetControllerType((HidControllerID) id1) & TYPE_JOYCON_RIGHT) {
                            lastRightId = id1;
                            // prevent missing player numbers
                            if (id0 < id1) {
                                hidMergeSingleJoyAsDualJoy((HidControllerID) id0, (HidControllerID) id1);
                            } else if (id0 > id1) {
                                hidMergeSingleJoyAsDualJoy((HidControllerID) id1, (HidControllerID) id0);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    return SDL2Input::update(rotate);
}

void SWITCHInput::setSingleJoyconMode(bool enable) {
    single_joycon_mode = enable;
}

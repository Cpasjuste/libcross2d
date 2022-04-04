//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

SWITCHInput::SWITCHInput() : SDL2Input() {
    padConfigureInput(8, HidNpadStyleSet_NpadStandard);
    padInitializeAny(&pad);

    //int mapLeft[KEY_COUNT] = {18, 16, 17, 19, 11, 6, 15, 12, 14, 13, 24, 25, 4, 8, 8, 4};
    //int mapRight[KEY_COUNT] = {20, 22, 23, 21, 10, 7, 2, 0, 3, 1, 26, 27, 5, 9, 9, 5};

    // TODO
    // set left joycon mapping
    mapLeft = {
            {Button::Up,     KEY_JOY_UP_DEFAULT},
            {Button::Down,   KEY_JOY_DOWN_DEFAULT},
            {Button::Left,   KEY_JOY_LEFT_DEFAULT},
            {Button::Right,  KEY_JOY_RIGHT_DEFAULT},
            {Button::Select, KEY_JOY_SELECT_DEFAULT},
            {Button::Start,  KEY_JOY_START_DEFAULT},
            {Button::A,      KEY_JOY_A_DEFAULT},
            {Button::B,      KEY_JOY_B_DEFAULT},
            {Button::X,      KEY_JOY_X_DEFAULT},
            {Button::Y,      KEY_JOY_Y_DEFAULT},
            {Button::LT,     KEY_JOY_LT_DEFAULT},
            {Button::RT,     KEY_JOY_RT_DEFAULT},
            {Button::LB,     KEY_JOY_LB_DEFAULT},
            {Button::RB,     KEY_JOY_RB_DEFAULT},
            {Button::LS,     KEY_JOY_LS_DEFAULT},
            {Button::RS,     KEY_JOY_RS_DEFAULT},
            {Button::Menu1,  KEY_JOY_MENU1_DEFAULT},
            {Button::Menu2,  KEY_JOY_MENU2_DEFAULT}
    };

    // TODO
    // set right joycon mapping
    mapRight = {
            {Button::Up,     KEY_JOY_UP_DEFAULT},
            {Button::Down,   KEY_JOY_DOWN_DEFAULT},
            {Button::Left,   KEY_JOY_LEFT_DEFAULT},
            {Button::Right,  KEY_JOY_RIGHT_DEFAULT},
            {Button::Select, KEY_JOY_SELECT_DEFAULT},
            {Button::Start,  KEY_JOY_START_DEFAULT},
            {Button::A,      KEY_JOY_A_DEFAULT},
            {Button::B,      KEY_JOY_B_DEFAULT},
            {Button::X,      KEY_JOY_X_DEFAULT},
            {Button::Y,      KEY_JOY_Y_DEFAULT},
            {Button::LT,     KEY_JOY_LT_DEFAULT},
            {Button::RT,     KEY_JOY_RT_DEFAULT},
            {Button::LB,     KEY_JOY_LB_DEFAULT},
            {Button::RB,     KEY_JOY_RB_DEFAULT},
            {Button::LS,     KEY_JOY_LS_DEFAULT},
            {Button::RS,     KEY_JOY_RS_DEFAULT},
            {Button::Menu1,  KEY_JOY_MENU1_DEFAULT},
            {Button::Menu2,  KEY_JOY_MENU2_DEFAULT}
    };
}

Input::Player *SWITCHInput::update() {
    Input::Player *players = SDL2Input::update();

    padUpdate(&pad);

    // handle joycon's states (@ rsn8887 && R-YaTian)
    static int previous_single_joycon_mode = -1;
    static int previous_handheld_mode = -1;
    handheld_mode = padIsHandheld(&pad);

    if ((single_joy_mode != previous_single_joycon_mode) || (handheld_mode != previous_handheld_mode)) {
        if (!handheld_mode) {
            if (single_joy_mode) {
                hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                for (int id = 0; id < 8; id++)
                    hidSetNpadJoyAssignmentModeSingleByDefault((HidNpadIdType) id);
            } else {
                for (int i = 0; i < 8; i += 2) {
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType) i);
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType) (i + 1));
                    hidMergeSingleJoyAsDualJoy((HidNpadIdType) i, (HidNpadIdType) (i + 1));
                }
            }
        }
        previous_handheld_mode = handheld_mode;
        previous_single_joycon_mode = single_joy_mode;
    }

    return players;
}

void SWITCHInput::setSingleJoyMode(bool enable) {
    single_joy_mode = enable;
}

Vector2f SWITCHInput::getAxisState(const Player &player, int xAxis, int yAxis) {
    if (!player.enabled || !player.data || (single_joy_mode && !handheld_mode)) {
        return {};
    }

    return SDL2Input::getAxisState(player, xAxis, yAxis);
}

// TODO
#if 0
void SWITCHInput::process_buttons(Input::Player &player, int rotate) {
    if (!player.enabled || !player.data) {
        return;
    }

    int button;
    std::vector<ButtonMapping> *map = &player.mapping;
    auto gc = (SDL_GameController *) player.data;
    auto joy = SDL_GameControllerGetJoystick(gc);

    if (single_joy_mode && !handheld_mode) {
        int index = (int) SDL_JoystickInstanceID(joy);
        if (hidGetNpadDeviceType((HidNpadIdType) index) & HidDeviceTypeBits_JoyLeft) {
            map = &mapLeft;
        } else if (hidGetNpadDeviceType((HidNpadIdType) index) & HidDeviceTypeBits_JoyRight) {
            map = &mapRight;
        }
    }

    for (const auto &buttonMap: *map) {
        button = buttonMap.value;

        // process shoulder buttons, this is a special case on switch as they are treated has digital buttons
        // SDL2 do not seem to handle this, so we use joystick code
        if (button == ((int) SDL_CONTROLLER_AXIS_TRIGGERLEFT + 100)) {
            if (SDL_JoystickGetButton(joy, 8)) {
                player.buttons |= buttonMap.button;
            }
        } else if (button == ((int) SDL_CONTROLLER_AXIS_TRIGGERRIGHT + 100)) {
            if (SDL_JoystickGetButton(joy, 9)) {
                player.buttons |= buttonMap.button;
            }
        } else if (SDL_GameControllerGetButton(gc, (SDL_GameControllerButton) button)) {
            player.buttons |= buttonMap.button;
        }
    }
}
#endif
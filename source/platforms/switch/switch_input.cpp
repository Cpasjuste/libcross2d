//
// Created by cpasjuste on 11/01/17.
//

#include "cross2d/c2d.h"
#include <cross2d/platforms/switch/switch_input.h>

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
        if (!handheld_mode)
        {
            if (single_joycon_mode)
            {
                hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                for (int id = 0; id < 8; id++)
                    hidSetNpadJoyAssignmentModeSingleByDefault((HidNpadIdType)id);
            }
            else
            {
                for (int i = 0; i < 8; i += 2)
                {
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType)i);
                    hidSetNpadJoyAssignmentModeDual((HidNpadIdType)(i + 1));
                    hidMergeSingleJoyAsDualJoy((HidNpadIdType)i, (HidNpadIdType)(i + 1));
                }
            }
        }
        previous_handheld_mode = handheld_mode;
        previous_single_joycon_mode = single_joycon_mode;
    }
    if (single_joycon_mode && !handheld_mode) {
        for (int i = 0; i < PLAYER_MAX; i++) {
            auto joystick = (SDL_Joystick *) players[i].data;
            int index = (int)SDL_JoystickInstanceID(joystick);
            if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyLeft) {
                if (SDL_JoystickGetButton(joystick, KEY_JOY_ZL_DEFAULT))
                    players[i].keys |= Input::Key::Select;
            }
            else if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyRight) {
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

void SWITCHInput::process_buttons(Input::Player& player, int rotate)
{
    if (!player.enabled || player.data == nullptr) {
        return;
    }

    int map[12] = { 0 };

    if (single_joycon_mode && !handheld_mode) 
    {
        auto joystick = (SDL_Joystick*)player.data;
        int index = (int)SDL_JoystickInstanceID(joystick);
        if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyLeft) {
            int mapLeft[] = { 18,16,17,19,11,6,15,12,14,13,24,25 };
            for (int j = 0; j < 12; j++)
            {
                map[j] = mapLeft[j];
            }
        }
        else if (hidGetNpadDeviceType((HidNpadIdType)index) & HidDeviceTypeBits_JoyRight) {
            int mapRight[] = { 20,22,23,21,10,7,2,0,3,1,26,27 };
            for (int j = 0; j < 12; j++)
            {
                map[j] = mapRight[j];
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        int mapping = 0;
        if (single_joycon_mode && !handheld_mode)
            mapping = map[i];
        else
            mapping = player.mapping[i];
        if (SDL_JoystickGetButton((SDL_Joystick*)player.data, mapping))
            player.keys |= key_id[i];
    }
}

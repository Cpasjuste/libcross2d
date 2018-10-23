//
// Created by cpasjuste on 11/01/17.
//

#ifndef C2D_INPUT_H
#define C2D_INPUT_H

#define PLAYER_COUNT 4
#define KEY_COUNT 12

#define EV_RESIZE 0x4000
#define EV_QUIT 0x8000
#define EV_REFRESH 0x10000
#define EV_DELAY 0x20000

namespace c2d {

    class Input {

    public:

        enum Key {
            KEY_UP = 0x0001,
            KEY_DOWN = 0x0002,
            KEY_LEFT = 0x0004,
            KEY_RIGHT = 0x0008,
            KEY_COIN = 0x0010,
            KEY_START = 0x0020,
            KEY_FIRE1 = 0x0040,
            KEY_FIRE2 = 0x0080,
            KEY_FIRE3 = 0x0100,
            KEY_FIRE4 = 0x0200,
            KEY_FIRE5 = 0x0400,
            KEY_FIRE6 = 0x0800
        };

        struct Axis {
            int id;
            short value;
        };

        struct Player {
            int mapping[KEY_COUNT];
            Axis lx{0, 0};
            Axis ly{1, 0};
            Axis rx{2, 0};
            Axis ry{3, 0};
            unsigned int state = 0;
            int dead_zone = 8000;
            bool enabled = false;
            void *data = nullptr;
            int id = 0;
        };

        // map keyboard to player 0
        struct Keyboard {
            int mapping[KEY_COUNT];
        };

        explicit Input();

        virtual ~Input();

        virtual Player *update(int rotate = 0); // to implement

        virtual int waitButton(int player = 0) { return -1; }; // to implement

        unsigned int getKeys(int player = 0);

        Player *getPlayer(int player = 0);

        Player *getPlayers();

        int clear(int player = 0);

        void setRepeatEnable(bool enable);

        void setRepeatDelay(int ms);

        void setJoystickMapping(int player, const int *mapping, int deadzone = 8000);

        void setKeyboardMapping(const int *mapping);

        Player players[PLAYER_COUNT];
        Keyboard keyboard;

    private:

        Clock *repeatClock;
        int repeatDelay = 150;
        bool repeat = false;
        unsigned int stateOld = 0;
    };
}

#endif //_INPUT_H

//
// Created by cpasjuste on 11/01/17.
//

#ifndef C2D_INPUT_H
#define C2D_INPUT_H

#ifndef BIT
#define BIT(n) (1U<<(n))
#endif

#define PLAYER_MAX 4
#define KEY_COUNT 12

#define EV_RESIZE   BIT(1)
#define EV_QUIT     BIT(2)

#include "cross2d/skeleton/sfml/Clock.hpp"
#include "cross2d/skeleton/sfml/Vector2.hpp"

namespace c2d {

    class Input {

    public:

        enum Key : unsigned int {
            Up = BIT(3),
            Down = BIT(4),
            Left = BIT(5),
            Right = BIT(6),
            Select = BIT(7),
            Start = BIT(8),
            Fire1 = BIT(9),
            Fire2 = BIT(10),
            Fire3 = BIT(11),
            Fire4 = BIT(12),
            Fire5 = BIT(13),
            Fire6 = BIT(14),
            Touch = BIT(15),
            Delay = BIT(16)
        };

        struct Axis {
            int id;
            short value;
        };

        struct Player {
            int mapping[KEY_COUNT]{};
            Axis lx{0, 0};
            Axis ly{1, 0};
            Axis rx{2, 0};
            Axis ry{3, 0};
            unsigned int keys = 0;
            int dead_zone = 8000;
            bool enabled = false;
            void *data = nullptr;
            int id = 0;
            // touch
            Vector2f touch;
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

        void setRepeatDelay(int ms);

        int getRepeatDelay();

        void setJoystickMapping(int player, const int *mapping, int deadzone = 8000);

        void setKeyboardMapping(const int *mapping);

        Player players[PLAYER_MAX];
        Keyboard keyboard;

    private:

        Clock *repeatClock;
        int repeatDelay = 150;
        bool repeat = false;
        unsigned int stateOld = 0;
    };
}

#endif //_INPUT_H

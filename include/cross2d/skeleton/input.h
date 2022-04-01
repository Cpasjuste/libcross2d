//
// Created by cpasjuste on 11/01/17.
//

#ifndef __C2D_INPUT_H__
#define __C2D_INPUT_H__

#include "cross2d/skeleton/sfml/Clock.hpp"
#include "cross2d/skeleton/sfml/Vector2.hpp"

#ifndef BIT
#define BIT(n) (1U<<(n))
#endif

#define PLAYER_MAX 4

namespace c2d {

    class Input {

    public:

        enum Button : unsigned int {
            Up = BIT(1),
            Down = BIT(2),
            Left = BIT(3),
            Right = BIT(4),
            Select = BIT(5),
            Start = BIT(6),
            A = BIT(7),
            B = BIT(8),
            X = BIT(9),
            Y = BIT(10),
            LT = BIT(11),
            RT = BIT(12),
            LB = BIT(13),
            RB = BIT(14),
            LS = BIT(15),
            RS = BIT(16),
            Menu1 = BIT(17),
            Menu2 = BIT(18),
            Touch = BIT(19),
            Delay = BIT(20),
            Quit = BIT(21)
        };

        struct ButtonMapping {
            Button button;
            int value;
        };

        struct Axis {
            int id;
            short value;
        };

        struct Player {
            std::vector<ButtonMapping> mapping;
            Axis lx{};
            Axis ly{};
            Axis rx{};
            Axis ry{};
            unsigned int buttons = 0;
            int dz = 8000;
            bool enabled = false;
            void *data = nullptr;
            int id = 0;
            // touch
            Vector2f touch;
        };

        // map keyboard to player 0
        struct Keyboard {
            std::vector<ButtonMapping> mapping;
        };

        explicit Input();

        virtual ~Input();

        virtual Player *update(int rotate = 0); // to implement

        virtual bool waitButton(unsigned int *key, int player = 0) { return false; }; // to implement

        virtual unsigned int getButtons(int player = 0);

        virtual Player *getPlayer(int player = 0);

        virtual Player *getPlayers();

        virtual int clear(int player = 0);

        virtual void setRepeatDelay(int ms);

        virtual int getRepeatDelay();

        virtual void setJoystickMapping(int player, const std::vector<ButtonMapping> &mapping,
                                        int lx, int ly, int rx, int ry, int dz);

        virtual void setKeyboardMapping(const std::vector<ButtonMapping> &mapping);

        Player players[PLAYER_MAX];
        Keyboard keyboard{};

    private:

        Clock *repeatClock;
        int repeatDelay = 150;
        bool repeat = false;
        unsigned int stateOld = 0;
    };
}

#endif //__C2D_INPUT_H__

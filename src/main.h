//
// Created by cpasjuste on 08/12/16.
//

#ifndef CROSS2D_MAIN_H
#define CROSS2D_MAIN_H

#ifdef __PSP2__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "app0:/default-20.pgf"
#define TEX_PATH    "app0:/title.png"
int KEYS[]{
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        82, 81, 80, 79, 89, 90, 91, 92, 93, 94, 41, 40, 0 // KEY_QUIT
};

#elif __3DS__

#define SCR_W 400
#define SCR_H 240
#define FONT_PATH   "/default.ttf"
#define TEX_PATH    "/title.png"
int KEYS[]{
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        6, 7, 5, 4, 2, 3, 0, 1, 10, 11, 9, 8, 0 // KEY_QUIT
};

#elif __NX__

#define SCR_W 1280
#define SCR_H 720
#define FONT_PATH   "default.ttf"
#define TEX_PATH    "title.png"

#elif __SFML__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "../src/res/default.ttf"
#define TEX_PATH    "../src/res/title.png"
int KEYS[]{
        // SFML_Scancode
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        73, 74, 71, 72, 36, 58, 91, 92, 93, 94, 41, 40, 0
};

#elif __PS3__

#define SCR_W 1280  // doesn't matter, virtual resolution used
#define SCR_H 720   // doesn't matter, virtual resolution used
#define FONT_PATH   "/dev_hdd0/default.ttf"
#define TEX_PATH    "/dev_hdd0/title.png"
int KEYS[]{
        // SDL_Scancode
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        82, 81, 80, 79, 89, 90, 91, 92, 93, 94, 41, 40, 0 // QUIT
};

#elif __SDL2__ || __SDL1__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "../src/res/default.ttf"
#define TEX_PATH    "../src/res/title.png"
int KEYS[]{
        // SDL_Scancode
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        82, 81, 80, 79, 89, 90, 91, 92, 93, 94, 41, 40, 0 // QUIT
};

#endif

#endif //CROSS2D_MAIN_H

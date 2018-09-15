//
// Created by cpasjuste on 08/12/16.
//

#ifndef CROSS2D_MAIN_H
#define CROSS2D_MAIN_H

int KEYS[]{
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        KEY_JOY_UP_DEFAULT, KEY_JOY_DOWN_DEFAULT, KEY_JOY_LEFT_DEFAULT, KEY_JOY_RIGHT_DEFAULT,
        KEY_JOY_COIN1_DEFAULT, KEY_JOY_START1_DEFAULT,
        KEY_JOY_FIRE1_DEFAULT, KEY_JOY_FIRE2_DEFAULT, KEY_JOY_FIRE3_DEFAULT,
        KEY_JOY_FIRE4_DEFAULT, KEY_JOY_FIRE5_DEFAULT, KEY_JOY_FIRE6_DEFAULT,
        0
        // 13, 15, 12, 14, 10, 11, 0, 1, 2, 3, 7, 6, 0 // QUIT
};

#ifdef __PSP2__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "app0:/default.ttf"
#define TEX_PATH    "app0:/title.png"

#elif __3DS__

#define SCR_W 400
#define SCR_H 240
#define FONT_PATH   "/default.ttf"
#define TEX_PATH    "/title.png"

#elif __SWITCH__

#define SCR_W 1280
#define SCR_H 720
#define FONT_PATH   "/switch/pfba/skin/default.ttf"
#define TEX_PATH    "/switch/pfba/skin/title.png"

#elif __SFML__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "../res/default.ttf"
#define TEX_PATH    "../res/title.png"

#elif __PS3__

#define SCR_W 1280  // doesn't matter, virtual resolution used
#define SCR_H 720   // doesn't matter, virtual resolution used
#define FONT_PATH   "/dev_hdd0/default.ttf"
#define TEX_PATH    "/dev_hdd0/title.png"

#elif __SDL2__ || __SDL1__

#define SCR_W 960
#define SCR_H 544
#define FONT_PATH   "../res/default.ttf"
#define TEX_PATH    "../res/title.png"

#endif

#endif //CROSS2D_MAIN_H

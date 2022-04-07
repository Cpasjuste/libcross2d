//
// Created by cpasjuste on 29/11/17.
//

#ifndef __C2D_H__
#define __C2D_H__

#include "cross2d/skeleton/object.h"
#include "cross2d/skeleton/io.h"
#include "cross2d/skeleton/input.h"
#include "cross2d/skeleton/audio.h"
#include "cross2d/skeleton/sfml/Rectangle.hpp"
#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/skeleton/sfml/CircleShape.hpp"
#include "cross2d/skeleton/sfml/ConvexShape.hpp"
#include "cross2d/skeleton/sfml/BMFont.hpp"
#include "cross2d/skeleton/sfml/Text.hpp"
#include "cross2d/skeleton/sfml/Sprite.hpp"
#include "cross2d/skeleton/sfml/AnimatedSprite.h"
#include "cross2d/skeleton/renderer.h"
#include "cross2d/skeleton/texture.h"
#include "cross2d/skeleton/utility.h"
#include "cross2d/skeleton/tween.h"
#include "cross2d/skeleton/config.h"
#include "cross2d/skeleton/thread.h"
#include "cross2d/skeleton/mutex.h"
#include "cross2d/skeleton/cond.h"
#include "cross2d/widgets/button.h"
#include "cross2d/widgets/textbox.h"
#include "cross2d/widgets/listbox.h"
#include "cross2d/widgets/configbox.h"
#include "cross2d/widgets/progress.h"
#include "cross2d/widgets/gradient_rectangle.h"

#ifdef __WINDOWS__
#undef MessageBox
#endif

#include "cross2d/widgets/messagebox.h"

#define C2DRectangle RectangleShape
#define C2DCircle CircleShape
#define C2DFont Font
#define C2DText Text

// for internal usage
extern c2d::Renderer *c2d_renderer;

#if defined(__SDL2__) || defined(__PSP2__)
#define KEY_JOY_UP_DEFAULT      SDL_CONTROLLER_BUTTON_DPAD_UP
#define KEY_JOY_DOWN_DEFAULT    SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define KEY_JOY_LEFT_DEFAULT    SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define KEY_JOY_RIGHT_DEFAULT   SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define KEY_JOY_A_DEFAULT       SDL_CONTROLLER_BUTTON_A
#define KEY_JOY_B_DEFAULT       SDL_CONTROLLER_BUTTON_B
#define KEY_JOY_X_DEFAULT       SDL_CONTROLLER_BUTTON_X
#define KEY_JOY_Y_DEFAULT       SDL_CONTROLLER_BUTTON_Y
#ifdef __PSP2__
#define KEY_JOY_LT_DEFAULT      SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define KEY_JOY_RT_DEFAULT      SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define KEY_JOY_LB_DEFAULT      SDL_CONTROLLER_BUTTON_INVALID
#define KEY_JOY_RB_DEFAULT      SDL_CONTROLLER_BUTTON_INVALID
#else
#define KEY_JOY_LT_DEFAULT      (SDL_CONTROLLER_AXIS_TRIGGERLEFT + 100)     // axis id should not conflict with button id
#define KEY_JOY_RT_DEFAULT      (SDL_CONTROLLER_AXIS_TRIGGERRIGHT + 100)    // axis id should not conflict with button id
#define KEY_JOY_LB_DEFAULT      SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define KEY_JOY_RB_DEFAULT      SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#endif
#define KEY_JOY_LS_DEFAULT      SDL_CONTROLLER_BUTTON_LEFTSTICK
#define KEY_JOY_RS_DEFAULT      SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define KEY_JOY_SELECT_DEFAULT  SDL_CONTROLLER_BUTTON_BACK
#define KEY_JOY_START_DEFAULT   SDL_CONTROLLER_BUTTON_START
#define KEY_JOY_MENU1_DEFAULT   SDL_CONTROLLER_BUTTON_START
#define KEY_JOY_MENU2_DEFAULT   SDL_CONTROLLER_BUTTON_BACK
#define KEY_JOY_AXIS_LX         SDL_CONTROLLER_AXIS_LEFTX
#define KEY_JOY_AXIS_LY         SDL_CONTROLLER_AXIS_LEFTY
#define KEY_JOY_AXIS_RX         SDL_CONTROLLER_AXIS_RIGHTX
#define KEY_JOY_AXIS_RY         SDL_CONTROLLER_AXIS_RIGHTY
#endif

#ifdef __PSP2__
#define NO_KEYBOARD 1

#include <psp2/kernel/clib.h>

#define printf sceClibPrintf

#include "platforms/psp2/psp2_renderer.h"
#include "platforms/psp2/psp2_texture.h"
#include "platforms/psp2/psp2_io.h"
#include "platforms/psp2/psp2_clock.h"
#include "platforms/sdl2/sdl2_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/sdl2/sdl2_thread.h"
#include "platforms/sdl2/sdl2_mutex.h"
#include "platforms/sdl2/sdl2_cond.h"

#define C2DRenderer PSP2Renderer
#define C2DTexture PSP2Texture
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo PSP2Io
#define C2DClock PSP2Clock
#define C2DThread SDL2Thread
#define C2DMutex SDL2Mutex
#define C2DCond SDL2Cond

#elif __PS4__
#ifndef NDEBUG

#include "cross2d/platforms/ps4/ps4_sys.h"

#define printf c2d::PS4Sys::print
#endif

#define GL_UNPACK_ROW_LENGTH 0x0CF2

#include "c2d_gl2.h"
#include "c2d_sdl2.h"
#include "cross2d/platforms/ps4/ps4_io.h"
#include "cross2d/platforms/ps4/ps4_clock.h"

#define C2DIo PS4Io
#define C2DClock PS4Clock

#elif __3DS__
#define NO_KEYBOARD 1

#include <3ds.h>
#include <citro3d.h>
#include "platforms/3ds/ctr_renderer.h"
#include "platforms/3ds/ctr_texture.h"
#include "platforms/3ds/ctr_input.h"
#include "platforms/3ds/ctr_io.h"
#include "platforms/3ds/ctr_clock.h"
#include "platforms/3ds/ctr_audio.h"
#include "platforms/3ds/ctr_thread.h"
#include "platforms/3ds/ctr_mutex.h"

#define C2DRenderer CTRRenderer
#define C2DTexture CTRTexture
#define C2DInput CTRInput
#define C2DAudio CTRAudio
#define C2DIo CTRIo
#define C2DClock CTRClock
#define C2DThread CTRThread
#define C2DMutex CTRMutex

// from <3ds/services/hid.h>
#define KEY_JOY_UP_DEFAULT      KEY_UP
#define KEY_JOY_DOWN_DEFAULT    KEY_DOWN
#define KEY_JOY_LEFT_DEFAULT    KEY_LEFT
#define KEY_JOY_RIGHT_DEFAULT   KEY_RIGHT
#define KEY_JOY_FIRE1_DEFAULT   KEY_A
#define KEY_JOY_FIRE2_DEFAULT   KEY_B
#define KEY_JOY_FIRE3_DEFAULT   KEY_X
#define KEY_JOY_FIRE4_DEFAULT   KEY_Y
#define KEY_JOY_FIRE5_DEFAULT   KEY_L
#define KEY_JOY_FIRE6_DEFAULT   KEY_R
#define KEY_JOY_FIRE7_DEFAULT   KEY_ZL
#define KEY_JOY_FIRE8_DEFAULT   KEY_ZR
#define KEY_JOY_COIN1_DEFAULT   KEY_SELECT
#define KEY_JOY_START1_DEFAULT  KEY_START
#define KEY_JOY_MENU1_DEFAULT   KEY_START
#define KEY_JOY_MENU2_DEFAULT   KEY_SELECT
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         0
#define KEY_JOY_AXIS_RX         0
#define KEY_JOY_AXIS_RY         0

#elif __SWITCH__
#define NO_KEYBOARD 1

#include <switch.h>

#include "c2d_gl2.h"
#include "c2d_sdl2.h"
#include "cross2d/platforms/posix/posix_io.h"
#include "cross2d/platforms/posix/posix_clock.h"

#include "platforms/switch/switch_renderer.h"
#include "platforms/switch/switch_io.h"
#include "platforms/switch/switch_sys.h"

#define C2DIo POSIXIo
#define C2DClock POSIXClock
#undef C2DRenderer
#define C2DRenderer SWITCHRenderer
#undef C2DIo
#define C2DIo NXIo
#elif __SDL2__

#if __GL1__

#include "c2d_gl1.h"

#elif __GL2__

#include "c2d_gl2.h"

#endif

#include "c2d_sdl2.h"
#include "cross2d/platforms/posix/posix_io.h"
#include "cross2d/platforms/posix/posix_clock.h"

#define C2DIo POSIXIo
#define C2DClock POSIXClock

#elif __SDL1__

#if __GL1__

#include "c2d_gl1.h"

#elif __GL2__

#include "c2d_gl2.h"

#endif

#include "c2d_sdl1.h"
#include "cross2d/platforms/posix/posix_io.h"
#include "cross2d/platforms/posix/posix_clock.h"

#define C2DIo POSIXIo
#define C2DClock POSIXClock

#define KEY_JOY_UP_DEFAULT      -1  // use hat
#define KEY_JOY_DOWN_DEFAULT    -1  // use hat
#define KEY_JOY_LEFT_DEFAULT    -1  // use hat
#define KEY_JOY_RIGHT_DEFAULT   -1  // use hat
#define KEY_JOY_FIRE1_DEFAULT   0
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   2
#define KEY_JOY_FIRE4_DEFAULT   3
#define KEY_JOY_FIRE5_DEFAULT   4
#define KEY_JOY_FIRE6_DEFAULT   5
#define KEY_JOY_COIN1_DEFAULT   6
#define KEY_JOY_START1_DEFAULT  7
#define KEY_JOY_MENU1_DEFAULT   7
#define KEY_JOY_MENU2_DEFAULT   6
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         4
#define KEY_JOY_AXIS_RY         5

#elif __DREAMCAST__

#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glkos.h>

#define GL_RGB565 GL_RGB

#include "platforms/gl1/gl_texture.h"
#include "platforms/dreamcast/dreamcast_renderer.h"
#include "platforms/dreamcast/dreamcast_clock.h"
#include "platforms/dreamcast/dreamcast_input.h"
#include "platforms/dreamcast/dreamcast_io.h"
#include "platforms/dreamcast/dreamcast_mutex.h"
#include "platforms/dreamcast/dreamcast_thread.h"
#include "platforms/dreamcast/dreamcast_audio.h"

#define C2DRenderer DCRenderer
#define C2DIo DCIo
#define C2DClock DCClock
#define C2DMutex DCMutex
#define C2DThread DCThread
#define C2DTexture GLTexture
#define C2DInput DCInput
#define C2DAudio DCAudio

#define KEY_JOY_UP_DEFAULT      CONT_DPAD_UP
#define KEY_JOY_DOWN_DEFAULT    CONT_DPAD_DOWN
#define KEY_JOY_LEFT_DEFAULT    CONT_DPAD_LEFT
#define KEY_JOY_RIGHT_DEFAULT   CONT_DPAD_RIGHT
#define KEY_JOY_FIRE1_DEFAULT   CONT_A
#define KEY_JOY_FIRE2_DEFAULT   CONT_B
#define KEY_JOY_FIRE3_DEFAULT   CONT_X
#define KEY_JOY_FIRE4_DEFAULT   CONT_Y
#define KEY_JOY_FIRE5_DEFAULT   CONT_C
#define KEY_JOY_FIRE6_DEFAULT   CONT_D
#define KEY_JOY_COIN1_DEFAULT   -1
#define KEY_JOY_START1_DEFAULT  CONT_START
#define KEY_JOY_MENU1_DEFAULT   -1
#define KEY_JOY_MENU2_DEFAULT   -1
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3

#elif __PS3__

#include "platforms/ps3/tiny3d_renderer.h"
#include "platforms/ps3/tiny3d_texture.h"
//#include "platforms/ps3/ps3_input.h"
//#include "platforms/ps3/ps3_audio.h"
#include "platforms/ps3/ps3_clock.h"
#include "platforms/ps3/ps3_mutex.h"
#include "platforms/posix/posix_io.h"

#define C2DRenderer TINY3DRenderer
#define C2DTexture TINY3DTexture
//#define C2DInput PS3Input
//#define C2DAudio PS3Audio
#define C2DClock PS3Clock
#define C2DMutex PS3Mutex
#define C2DInput Input
#define C2DAudio Audio
#define C2DIo POSIXIo

#define KEY_JOY_UP_DEFAULT      3
#define KEY_JOY_DOWN_DEFAULT    1
#define KEY_JOY_LEFT_DEFAULT    0
#define KEY_JOY_RIGHT_DEFAULT   2
#define KEY_JOY_FIRE1_DEFAULT   9
#define KEY_JOY_FIRE2_DEFAULT   10
#define KEY_JOY_FIRE3_DEFAULT   8
#define KEY_JOY_FIRE4_DEFAULT   11
#define KEY_JOY_FIRE5_DEFAULT   13
#define KEY_JOY_FIRE6_DEFAULT   12
#define KEY_JOY_COIN1_DEFAULT   7
#define KEY_JOY_START1_DEFAULT  4
#define KEY_JOY_MENU1_DEFAULT   4
#define KEY_JOY_MENU2_DEFAULT   7
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3

#elif __SFML__

#include "platforms/sfml/sfml_renderer.h"
#include "platforms/sfml/sfml_texture.h"
#include "platforms/sfml/sfml_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/posix/posix_io.h"

#define C2DRenderer SFMLRenderer
#define C2DTexture SFMLTexture
#define C2DRectangle Rectangle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput SFMLInput
#define C2DAudio SDL2Audio
#define C2DIo POSIXIo

#define KEY_JOY_UP_DEFAULT      -1  // use hat
#define KEY_JOY_DOWN_DEFAULT    -1  // use hat
#define KEY_JOY_LEFT_DEFAULT    -1  // use hat
#define KEY_JOY_RIGHT_DEFAULT   -1  // use hat
#define KEY_JOY_FIRE1_DEFAULT   0
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   2
#define KEY_JOY_FIRE4_DEFAULT   3
#define KEY_JOY_FIRE5_DEFAULT   4
#define KEY_JOY_FIRE6_DEFAULT   5
#define KEY_JOY_COIN1_DEFAULT   6
#define KEY_JOY_START1_DEFAULT  7
#define KEY_JOY_MENU1_DEFAULT   7
#define KEY_JOY_MENU2_DEFAULT   6
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         4
#define KEY_JOY_AXIS_RY         5

#endif

#ifndef NO_KEYBOARD
#if defined(__SDL2__)
// SDL key.keysym.scancode
#define KEY_KB_UP_DEFAULT       SDL_SCANCODE_UP
#define KEY_KB_DOWN_DEFAULT     SDL_SCANCODE_DOWN
#define KEY_KB_LEFT_DEFAULT     SDL_SCANCODE_LEFT
#define KEY_KB_RIGHT_DEFAULT    SDL_SCANCODE_RIGHT
#define KEY_KB_A_DEFAULT        SDL_SCANCODE_A
#define KEY_KB_B_DEFAULT        SDL_SCANCODE_S
#define KEY_KB_X_DEFAULT        SDL_SCANCODE_Q
#define KEY_KB_Y_DEFAULT        SDL_SCANCODE_W
#define KEY_KB_LT_DEFAULT       SDL_SCANCODE_E
#define KEY_KB_RT_DEFAULT       SDL_SCANCODE_D
#define KEY_KB_LB_DEFAULT       SDL_SCANCODE_R
#define KEY_KB_RB_DEFAULT       SDL_SCANCODE_F
#define KEY_KB_LS_DEFAULT       SDL_SCANCODE_T
#define KEY_KB_RS_DEFAULT       SDL_SCANCODE_G
#define KEY_KB_SELECT_DEFAULT   SDL_SCANCODE_SPACE
#define KEY_KB_START_DEFAULT    SDL_SCANCODE_RETURN
#define KEY_KB_MENU1_DEFAULT    SDL_SCANCODE_RETURN
#define KEY_KB_MENU2_DEFAULT    SDL_SCANCODE_SPACE
#elif defined(__SDL1__)
// SDLKey from :
#include <SDL/SDL_keysym.h>

#define KEY_KB_UP_DEFAULT      SDLK_UP
#define KEY_KB_DOWN_DEFAULT    SDLK_DOWN
#define KEY_KB_LEFT_DEFAULT    SDLK_LEFT
#define KEY_KB_RIGHT_DEFAULT   SDLK_RIGHT
#define KEY_KB_FIRE1_DEFAULT   SDLK_a
#define KEY_KB_FIRE2_DEFAULT   SDLK_z
#define KEY_KB_FIRE3_DEFAULT   SDLK_e
#define KEY_KB_FIRE4_DEFAULT   SDLK_q
#define KEY_KB_FIRE5_DEFAULT   SDLK_s
#define KEY_KB_FIRE6_DEFAULT   SDLK_d
#define KEY_KB_COIN1_DEFAULT   SDLK_SPACE
#define KEY_KB_START1_DEFAULT  SDLK_RETURN
#define KEY_KB_MENU1_DEFAULT   SDLK_SPACE
#define KEY_KB_MENU2_DEFAULT   SDLK_RETURN
#elif __SFML__
#define KEY_KB_UP_DEFAULT      73
#define KEY_KB_DOWN_DEFAULT    74
#define KEY_KB_LEFT_DEFAULT    71
#define KEY_KB_RIGHT_DEFAULT   72
#define KEY_KB_FIRE1_DEFAULT   76
#define KEY_KB_FIRE2_DEFAULT   77
#define KEY_KB_FIRE3_DEFAULT   78
#define KEY_KB_FIRE4_DEFAULT   79
#define KEY_KB_FIRE5_DEFAULT   80
#define KEY_KB_FIRE6_DEFAULT   81
#define KEY_KB_COIN1_DEFAULT   36
#define KEY_KB_START1_DEFAULT  58
#define KEY_KB_MENU1_DEFAULT   58
#define KEY_KB_MENU2_DEFAULT   36
#endif
#endif

#endif //__C2D_H__

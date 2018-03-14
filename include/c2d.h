//
// Created by cpasjuste on 29/11/17.
//

#ifndef _C2D_H_
#define _C2D_H_

#include "skeleton/object.h"
#include "skeleton/io.h"
#include "skeleton/input.h"
#include "skeleton/audio.h"
#include "skeleton/renderer.h"
#include "skeleton/rectangle.h"
#include "skeleton/line.h"
#include "skeleton/circle.h"
#include "skeleton/texture.h"
#include "skeleton/text.h"
#include "widgets/button.h"
#include "widgets/textbox.h"
#include "widgets/listbox.h"
#include "widgets/messagebox.h"

// for internal usage
extern c2d::Renderer *c2d_renderer;

#ifdef __PSP2__

#ifdef __PSP2_DEBUG__
#include <psp2/kernel/clib.h>
#define printf sceClibPrintf
#endif

#include "platforms/psp2/psp2_renderer.h"
#include "platforms/psp2/psp2_texture.h"
#include "platforms/psp2/psp2_io.h"
#include "platforms/psp2/psp2_clock.h"
#include "platforms/sdl2/sdl2_input.h"
#include "platforms/sdl2/sdl2_audio.h"

#define C2DRenderer PSP2Renderer
#define C2DTexture PSP2Texture
#define C2DRectangle Rectangle
#define C2DCircle Circle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo PSP2Io
#define C2DClock PSP2Clock

#define KEY_JOY_UP_DEFAULT      8
#define KEY_JOY_DOWN_DEFAULT    6
#define KEY_JOY_LEFT_DEFAULT    7
#define KEY_JOY_RIGHT_DEFAULT   9
#define KEY_JOY_FIRE1_DEFAULT   2
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   3
#define KEY_JOY_FIRE4_DEFAULT   0
#define KEY_JOY_FIRE5_DEFAULT   4
#define KEY_JOY_FIRE6_DEFAULT   5
#define KEY_JOY_COIN1_DEFAULT   10
#define KEY_JOY_START1_DEFAULT  11
#define KEY_JOY_MENU1_DEFAULT   11
#define KEY_JOY_MENU2_DEFAULT   10
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3

#elif __3DS__

#include <3ds.h>
#include <citro3d.h>
#include "platforms/3ds/ctr_renderer.h"
#include "platforms/3ds/ctr_texture.h"
#include "platforms/3ds/ctr_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/posix/posix_io.h"

#define C2DRenderer CTRRenderer
#define C2DTexture CTRTexture
#define C2DRectangle Rectangle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput CTRInput
#define C2DAudio CTRAudio
#define C2DIo POSIXIo

// from <3ds/services/hid.h>
#define KEY_JOY_UP_DEFAULT      6
#define KEY_JOY_DOWN_DEFAULT    7
#define KEY_JOY_LEFT_DEFAULT    5
#define KEY_JOY_RIGHT_DEFAULT   4
#define KEY_JOY_FIRE1_DEFAULT   0
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   10
#define KEY_JOY_FIRE4_DEFAULT   11
#define KEY_JOY_FIRE5_DEFAULT   9
#define KEY_JOY_FIRE6_DEFAULT   8
#define KEY_JOY_COIN1_DEFAULT   2
#define KEY_JOY_START1_DEFAULT  3
#define KEY_JOY_MENU1_DEFAULT   3
#define KEY_JOY_MENU2_DEFAULT   2
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         0
#define KEY_JOY_AXIS_RX         0
#define KEY_JOY_AXIS_RY         0

#elif __NX__

#include "platforms/sdl2/sdl2_renderer.h"
#include "platforms/sdl2/sdl2_texture.h"
#include "platforms/sdl2/sdl2_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/nx/nx_clock.h"
#include "platforms/posix/posix_io.h"

#include <switch.h>

//#define SVC_DEBUG 1
//#define NET_DEBUG 1
#ifdef NET_DEBUG
#include "nxnetprint.h"
//#define NET_DEBUG_IP "192.168.0.13"
#define NET_DEBUG_IP "192.168.1.100"
//#define NET_DEBUG_IP "192.168.0.10"
#endif

#define C2DRenderer SDL2Renderer
#define C2DTexture SDL2Texture
#define C2DRectangle Rectangle
#define C2DCircle Circle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo POSIXIo
#define C2DClock NXClock

// https://github.com/devkitPro/SDL/blob/a8fa9ec02c3c77106207cd84fc230523c5c30c81/src/joystick/switch/SDL_sysjoystick.c#L51

#define KEY_JOY_UP_DEFAULT      13          // KEY_UP
#define KEY_JOY_DOWN_DEFAULT    15          // KEY_DOWN
#define KEY_JOY_LEFT_DEFAULT    12          // KEY_LEFT
#define KEY_JOY_RIGHT_DEFAULT   14          // KEY_RIGHT
#define KEY_JOY_FIRE1_DEFAULT   0           // KEY_A
#define KEY_JOY_FIRE2_DEFAULT   1           // KEY_B
#define KEY_JOY_FIRE3_DEFAULT   2           // KEY_X
#define KEY_JOY_FIRE4_DEFAULT   3           // KEY_Y
#define KEY_JOY_FIRE5_DEFAULT   6           // KEY_L
#define KEY_JOY_FIRE6_DEFAULT   7           // KEY_R
#define KEY_JOY_COIN1_DEFAULT   11          // KEY_PLUS
#define KEY_JOY_START1_DEFAULT  10          // KEY_MINUS
#define KEY_JOY_MENU1_DEFAULT   10          // KEY_MINUS
#define KEY_JOY_MENU2_DEFAULT   11          // KEY_PLUS
// switch special keys
#define KEY_JOY_ZL_DEFAULT      8           // KEY_ZL
#define KEY_JOY_ZR_DEFAULT      9           // KEY_ZR
#define KEY_JOY_SL_DEFAULT      24          // KEY_SL
#define KEY_JOY_SR_DEFAULT      25          // KEY_SR
// TODO
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3

#elif __SDL2__

#include <SDL2/SDL.h>

#if defined(__SDL2_GL__)
#include "platforms/sdl2/sdl2_gl_renderer.h"
#include "platforms/gl/gl_texture.h"
#define C2DTexture GLTexture
#else
#include "platforms/sdl2/sdl2_renderer.h"
#include "platforms/sdl2/sdl2_texture.h"
#define C2DTexture SDL2Texture
#endif

#include "platforms/sdl2/sdl2_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/posix/posix_io.h"
#include "platforms/posix/posix_clock.h"

#define C2DRenderer SDL2Renderer
#define C2DRectangle Rectangle
#define C2DCircle Circle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
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

#elif __SDL1__
#include "platforms/sdl1/sdl1_gl_renderer.h"
#include "platforms/gl/gl_texture.h"
#include "platforms/sdl1/sdl1_input.h"
#include "platforms/sdl1/sdl1_audio.h"
#include "platforms/posix/posix_io.h"
#include "platforms/posix/posix_clock.h"

#define C2DRenderer SDL1Renderer
#define C2DTexture GLTexture
#define C2DRectangle Rectangle
#define C2DCircle Circle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput SDL1Input
#define C2DAudio SDL1Audio
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

#elif __PS3__

#include "platforms/ps3/tiny3d_renderer.h"
#include "platforms/ps3/tiny3d_texture.h"
#include "platforms/ps3/ps3_input.h"
#include "platforms/ps3/ps3_audio.h"
#include "platforms/posix/posix_io.h"

#define C2DRenderer TINY3DRenderer
#define C2DTexture TINY3DTexture
#define C2DRectangle Rectangle
#define C2DLine Line
#define C2DFont Font
#define C2DText Text
#define C2DInput PS3Input
#define C2DAudio PS3Audio
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
#define KEY_KB_UP_DEFAULT      82
#define KEY_KB_DOWN_DEFAULT    81
#define KEY_KB_LEFT_DEFAULT    80
#define KEY_KB_RIGHT_DEFAULT   79
#define KEY_KB_FIRE1_DEFAULT   20   // A
#define KEY_KB_FIRE2_DEFAULT   26   // Z
#define KEY_KB_FIRE3_DEFAULT   8    // E
#define KEY_KB_FIRE4_DEFAULT   4    // Q
#define KEY_KB_FIRE5_DEFAULT   22   // S
#define KEY_KB_FIRE6_DEFAULT   7    // D
#define KEY_KB_COIN1_DEFAULT   44   // SPACE
#define KEY_KB_START1_DEFAULT  40   // ENTER
#define KEY_KB_MENU1_DEFAULT   40   // ENTER
#define KEY_KB_MENU2_DEFAULT   44   // SPACE
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

#endif //_C2D_H_

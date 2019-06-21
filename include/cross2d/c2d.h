//
// Created by cpasjuste on 29/11/17.
//

#ifndef _C2D_H_
#define _C2D_H_

#include "cross2d/skeleton/object.h"
#include "cross2d/skeleton/io.h"
#include "cross2d/skeleton/input.h"
#include "cross2d/skeleton/audio.h"
#include "cross2d/skeleton/sfml/Rectangle.hpp"
#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/skeleton/sfml/RoundedRectangleShape.h"
#include "cross2d/skeleton/sfml/CircleShape.hpp"
#include "cross2d/skeleton/sfml/ConvexShape.hpp"
#include "cross2d/skeleton/sfml/Text.hpp"
#include "cross2d/skeleton/sfml/Sprite.hpp"
#include "cross2d/skeleton/renderer.h"
#include "cross2d/skeleton/texture.h"
#include "cross2d/skeleton/utility.h"
#include "cross2d/skeleton/tween.h"
#include "cross2d/skeleton/config.h"
#include "cross2d/widgets/button.h"
#include "cross2d/widgets/textbox.h"
#include "cross2d/widgets/listbox.h"
#include "cross2d/widgets/configbox.h"
#include "cross2d/widgets/messagebox.h"
#include "cross2d/widgets/progress.h"

// for internal usage
extern c2d::Renderer *c2d_renderer;

#ifdef __PSP2__
#define C2D_SCREEN_WIDTH 960
#define C2D_SCREEN_HEIGHT 544
#define NO_KEYBOARD 1
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
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo PSP2Io
#define C2DClock PSP2Clock
#define C2DRectangle RectangleShape
#define C2DRoundedRectangle RoundedRectangleShape
#define C2DCircle CircleShape
#define C2DFont Font
#define C2DText Text

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
#define C2D_SCREEN_WIDTH 400
#define C2D_SCREEN_HEIGHT 240
#define NO_KEYBOARD 1

#include <3ds.h>
#include <citro3d.h>
#include "platforms/3ds/ctr_renderer.h"
#include "platforms/3ds/ctr_texture.h"
#include "platforms/3ds/ctr_input.h"
#include "platforms/3ds/ctr_io.h"
#include "platforms/3ds/ctr_clock.h"
#include "platforms/sdl2/sdl2_audio.h"

#define C2DRenderer CTRRenderer
#define C2DTexture CTRTexture
#define C2DInput CTRInput
#define C2DAudio Audio
#define C2DIo CTRIo
#define C2DClock CTRClock
#define C2DRectangle RectangleShape
#define C2DRoundedRectangle RoundedRectangleShape
#define C2DCircle CircleShape
#define C2DFont Font
#define C2DText Text

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

#elif __SWITCH__
#define C2D_SCREEN_WIDTH 1280
#define C2D_SCREEN_HEIGHT 720
#define NO_KEYBOARD 1
#define GL_ABGR_EXT 0x8000
//#define __SVC_DEBUG__ 1
//#define __NET_DEBUG__ 1

#include <switch.h>
#include <glad/glad.h>
#include "platforms/switch/switch_renderer.h"
#include "platforms/gl/gl_shaders.h"
#include "platforms/gl/gl_texture.h"
#include "platforms/gl/gl_texture_buffer.h"
#include "platforms/sdl2/sdl2_input.h"
#include "platforms/sdl2/sdl2_audio.h"
#include "platforms/switch/switch_io.h"
#include "platforms/switch/switch_sys.h"
#include "platforms/switch/switch_input.h"
#include "platforms/posix/posix_clock.h"

#define C2DRenderer SWITCHRenderer
#define C2DTexture GLTexture
#define C2DRectangle RectangleShape
#define C2DRoundedRectangle RoundedRectangleShape
#define C2DCircle CircleShape
#define C2DFont Font
#define C2DText Text
#define C2DInput SWITCHInput
#define C2DAudio SDL2Audio
#define C2DClock POSIXClock
#define C2DIo NXIo

// https://github.com/devkitPro/SDL/blob/switch-sdl2/src/joystick/switch/SDL_sysjoystick.c#L52
#define KEY_JOY_UP_DEFAULT      13          // KEY_DUP
#define KEY_JOY_DOWN_DEFAULT    15          // KEY_DDOWN
#define KEY_JOY_LEFT_DEFAULT    12          // KEY_DLEFT
#define KEY_JOY_RIGHT_DEFAULT   14          // KEY_DRIGHT
#define KEY_JOY_FIRE1_DEFAULT   0           // KEY_A
#define KEY_JOY_FIRE2_DEFAULT   1           // KEY_B
#define KEY_JOY_FIRE3_DEFAULT   2           // KEY_X
#define KEY_JOY_FIRE4_DEFAULT   3           // KEY_Y
#define KEY_JOY_FIRE5_DEFAULT   6           // KEY_L
#define KEY_JOY_FIRE6_DEFAULT   7           // KEY_R
#define KEY_JOY_COIN1_DEFAULT   11          // KEY_MINUS
#define KEY_JOY_START1_DEFAULT  10          // KEY_PLUS
// switch special keys
#define KEY_JOY_ZL_DEFAULT      8           // KEY_ZL
#define KEY_JOY_ZR_DEFAULT      9           // KEY_ZR
#define KEY_JOY_LSTICK_DEFAULT  4           // KEY_LSTICK
#define KEY_JOY_RSTICK_DEFAULT  5           // KEY_RSTICK
// joysticks axis
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3
#elif __SDL2__
#ifdef __LDK__
#define C2D_SCREEN_WIDTH 320
#define C2D_SCREEN_HEIGHT 480
#else
#define C2D_SCREEN_WIDTH 1280
#define C2D_SCREEN_HEIGHT 720
#endif
#include <SDL2/SDL.h>

#if defined(__SDL2_GL__)
#define GL_GLEXT_PROTOTYPES 1
#ifdef __WINDOWS__
#include <GL/glew.h>
#else
#ifdef __SDL2_GLES__
#include <SDL2/SDL_opengles2.h>
#define GL_RGBA8 GL_RGBA
#define GL_QUADS 0x0006
#else
#include <SDL2/SDL_opengl.h>
#endif
#endif
#include "cross2d/platforms/sdl2/sdl2_gl_renderer.h"
#include "cross2d/platforms/gl/gl_shaders.h"
#include "cross2d/platforms/gl/gl_texture.h"
#include "cross2d/platforms/gl/gl_texture_buffer.h"

#define C2DRenderer SDL2Renderer
#define C2DTexture GLTexture
#else
#include "platforms/sdl2/sdl2_renderer.h"
#include "platforms/sdl2/sdl2_texture.h"
#define C2DRenderer SDL2Renderer
#define C2DTexture SDL2Texture
#endif

#include "cross2d/platforms/sdl2/sdl2_input.h"
#include "cross2d/platforms/sdl2/sdl2_audio.h"
#include "cross2d/platforms/posix/posix_io.h"
#include "cross2d/platforms/posix/posix_clock.h"

#define C2DRectangle RectangleShape
#define C2DRoundedRectangle RoundedRectangleShape
#define C2DCircle CircleShape
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
#define C2D_SCREEN_WIDTH 1280
#define C2D_SCREEN_HEIGHT 720

#include "platforms/sdl1/sdl1_renderer.h"
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
#define C2D_SCREEN_WIDTH 1280
#define C2D_SCREEN_HEIGHT 720

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
#define C2D_SCREEN_WIDTH 1280
#define C2D_SCREEN_HEIGHT 720

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
#ifdef __LDK__
// SDL key.keysym.scancode
#define KEY_KB_UP_DEFAULT      82
#define KEY_KB_DOWN_DEFAULT    81
#define KEY_KB_LEFT_DEFAULT    80
#define KEY_KB_RIGHT_DEFAULT   79
#define KEY_KB_FIRE1_DEFAULT   226  // A
#define KEY_KB_FIRE2_DEFAULT   224  // Z
#define KEY_KB_FIRE3_DEFAULT   225  // E
#define KEY_KB_FIRE4_DEFAULT   44   // Q
#define KEY_KB_FIRE5_DEFAULT   43   // S
#define KEY_KB_FIRE6_DEFAULT   42   // D
#define KEY_KB_COIN1_DEFAULT   41   // SPACE
#define KEY_KB_START1_DEFAULT  40   // ENTER
#define KEY_KB_MENU1_DEFAULT   40   // ENTER
#define KEY_KB_MENU2_DEFAULT   41   // SPACE
#else
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
#endif
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

const int C2D_DEFAULT_JOY_KEYS[]{
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        KEY_JOY_UP_DEFAULT, KEY_JOY_DOWN_DEFAULT, KEY_JOY_LEFT_DEFAULT, KEY_JOY_RIGHT_DEFAULT,
        KEY_JOY_COIN1_DEFAULT, KEY_JOY_START1_DEFAULT,
        KEY_JOY_FIRE1_DEFAULT, KEY_JOY_FIRE2_DEFAULT, KEY_JOY_FIRE3_DEFAULT,
        KEY_JOY_FIRE4_DEFAULT, KEY_JOY_FIRE5_DEFAULT, KEY_JOY_FIRE6_DEFAULT,
        0
};

#ifndef NO_KEYBOARD
const int C2D_DEFAULT_KB_KEYS[]{
        // UP, DOWN, LEFT, RIGHT, COINS (SELECT), START, ..., // QUIT
        KEY_KB_UP_DEFAULT, KEY_KB_DOWN_DEFAULT, KEY_KB_LEFT_DEFAULT, KEY_KB_RIGHT_DEFAULT,
        KEY_KB_COIN1_DEFAULT, KEY_KB_START1_DEFAULT,
        KEY_KB_FIRE1_DEFAULT, KEY_KB_FIRE2_DEFAULT, KEY_KB_FIRE3_DEFAULT,
        KEY_KB_FIRE4_DEFAULT, KEY_KB_FIRE5_DEFAULT, KEY_KB_FIRE6_DEFAULT,
        0
        // 13, 15, 12, 14, 10, 11, 0, 1, 2, 3, 7, 6, 0 // QUIT
};
#else
const int C2D_DEFAULT_KB_KEYS[]{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
#endif

#endif //_C2D_H_

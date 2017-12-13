//
// Created by cpasjuste on 29/11/17.
//

#ifndef _C2D_H_
#define _C2D_H_

#include "skeleton/renderer.h"
#include "skeleton/texture.h"
//#include "skeleton/font.h"
//#include "skeleton/input.h"
//#include "skeleton/audio.h"
#include "skeleton/timer.h"
#include "skeleton/io.h"

#ifdef __PSP2__

#include "psp2/psp2_renderer.h"
#include "psp2/psp2_texture.h"
#include "psp2/psp2_font.h"
#include "sdl2/sdl2_input.h"
#include "sdl2/sdl2_audio.h"

#define C2DRenderer PSP2Renderer
#define C2DTexture PSP2Texture
#define C2DFont PSP2Font
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo PSP2Io

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
#include "3ds/ctr_renderer.h"
#include "3ds/ctr_texture.h"
#include "3ds/ctr_font.h"
#include "3ds/ctr_input.h"
#include "sdl2/sdl2_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer CTRRenderer
#define C2DTexture CTRTexture
#define C2DFont CTRFont
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

#include "nx/nx_renderer.h"
#include "nx/nx_font.h"
#define C2DRenderer NXRenderer
#define C2DIo NXIo

#elif __SDL2__

#include "sdl2/sdl2_renderer.h"
//#include "sdl2/sdl2_font.h"
//#include "sdl2/sdl2_input.h"
//#include "sdl2/sdl2_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer SDL2Renderer
#define C2DTexture SDL2Texture
#define C2DFont SDL2Font
#define C2DInput SDL2Input
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

#elif __SDL1__

#include "sdl1/sdl1_renderer.h"
#include "sdl1/sdl1_font.h"
#include "sdl1/sdl1_input.h"
#include "sdl1/sdl1_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer SDL1Renderer
#define C2DTexture SDL1Texture
#define C2DFont SDL1Font
#define C2DInput SDL1Input
#define C2DAudio SDL1Audio
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

#elif __PS3__

#include "ps3/tiny3d_renderer.h"
#include "ps3/tiny3d_texture.h"
#include "ps3/tiny3d_font.h"
#include "ps3/ps3_input.h"
#include "ps3/ps3_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer TINY3DRenderer
#define C2DTexture TINY3DTexture
#define C2DFont TINY3DFont
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

#include "sfml/sfml_renderer.h"
#include "sfml/sfml_texture.h"
#include "sfml/sfml_font.h"
#include "sfml/sfml_input.h"
#include "sdl2/sdl2_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer SFMLRenderer
#define C2DTexture SFMLTexture
#define C2DFont SFMLFont
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
#ifdef __SDL2__
#define KEY_KB_UP_DEFAULT      82
#define KEY_KB_DOWN_DEFAULT    81
#define KEY_KB_LEFT_DEFAULT    80
#define KEY_KB_RIGHT_DEFAULT   79
#define KEY_KB_FIRE1_DEFAULT   89
#define KEY_KB_FIRE2_DEFAULT   90
#define KEY_KB_FIRE3_DEFAULT   91
#define KEY_KB_FIRE4_DEFAULT   92
#define KEY_KB_FIRE5_DEFAULT   93
#define KEY_KB_FIRE6_DEFAULT   94
#define KEY_KB_COIN1_DEFAULT   44
#define KEY_KB_START1_DEFAULT  40
#define KEY_KB_MENU1_DEFAULT   40
#define KEY_KB_MENU2_DEFAULT   44
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

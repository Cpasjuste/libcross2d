//
// Created by cpasjuste on 29/11/17.
//

#ifndef _C2D_H_
#define _C2D_H_

#include "skeleton/renderer.h"
#include "skeleton/texture.h"
#include "skeleton/font.h"
#include "skeleton/input.h"
#include "skeleton/audio.h"
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

#elif __NX__

#include "nx/nx_renderer.h"
#include "nx/nx_font.h"
#define C2DRenderer NXRenderer
#define C2DIo NXIo

#elif __SDL2__

#include "sdl2/sdl2_renderer.h"
#include "sdl2/sdl2_font.h"
#include "sdl2/sdl2_input.h"
#include "sdl2/sdl2_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer SDL2Renderer
#define C2DTexture SDL2Texture
#define C2DFont SDL2Font
#define C2DInput SDL2Input
#define C2DAudio SDL2Audio
#define C2DIo POSIXIo

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

#elif __TINY3D__

#include "ps3/tiny3d_renderer.h"
#include "ps3/tiny3d_texture.h"
#include "sdl1/sdl1_font.h"
#include "sdl1/sdl1_input.h"
#include "sdl1/sdl1_audio.h"
#include "posix/posix_io.h"

#define C2DRenderer TINY3DRenderer
#define C2DTexture TINY3DTexture
#define C2DFont Font
#define C2DInput Input
#define C2DAudio Audio
#define C2DIo POSIXIo

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

#endif

#endif //_C2D_H_

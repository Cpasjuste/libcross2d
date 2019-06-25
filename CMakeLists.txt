cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
option(PLATFORM_LINUX "Linux platform target" OFF)
option(PLATFORM_WINDOWS "Windows platform target (msys/mingw64)" OFF)
option(PLATFORM_RPI3 "RIP3 platform target" OFF)
option(PLATFORM_SWITCH "Nintendo Switch platform target" OFF)
option(PLATFORM_3DS "Nintendo 3DS platform target" OFF)
option(PLATFORM_VITA "Sony PS Vita platform target" OFF)
option(PLATFORM_LDK "LDK Handheld platform target (mips: LDK, RS-97...)" OFF)
####################
# TARGET PLATFORM
####################

####################
# BUILD OPTIONS
####################
option(OPTION_SDL1 "SDL1 support (software, slow, unsupported)" OFF)
option(OPTION_SDL2_GL "SDL2 support (OpenGL 3.3)" OFF)
option(OPTION_SDL2_GLES2 "SDL2 support (OpenGLES 2.0)" OFF)
option(OPTION_SDL2_SOFT "SDL2 support (software, slow)" OFF)
option(OPTION_BUILTIN_LIBCONFIG "Build libconfig from sources" OFF)
option(OPTION_RECALBOX_BUILDROOT "Build from recalbox buildroot" OFF)
####################
# BUILD OPTIONS
####################

####################
# SANITY CHECKS
####################
if (PLATFORM_LINUX OR PLATFORM_WINDOWS OR PLATFORM_SWITCH)
    if (NOT (OPTION_SDL1 OR OPTION_SDL2_GL OR OPTION_SDL2_GLES2 OR OPTION_SDL2_SOFT))
        message(STATUS "C2D: SDL2 OpenGL 3.3 support enabled")
        set(OPTION_SDL2_GL ON CACHE BOOL "SDL2 support (OpenGL 3.3)" FORCE)
    endif ()
endif ()

if (PLATFORM_RPI3 OR OPTION_SDL2_GLES2)
    message(STATUS "C2D: SDL2 OpenGLES 2.0 support enabled")
    set(OPTION_SDL2_GLES2 ON CACHE BOOL "SDL2 support (OpenGLES 2.0)" FORCE)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()
####################
# SANITY CHECKS
####################

# setup target toolchain
include(Toolchain.cmake)

project(cross2d)

####################
# COMMON STUFF
####################
file(GLOB C2D_SOURCES
        source/widgets/*.c*
        source/skeleton/*.c*
        source/skeleton/sfml/*.c*)
include(FindPkgConfig)
pkg_search_module(ZLIB REQUIRED zlib)
pkg_search_module(FREETYPE REQUIRED freetype2)
set(C2D_INCLUDES include ${FREETYPE_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(C2D_CFLAGS -Wall -Wno-narrowing -DLIBCONFIG_STATIC)
set(C2D_LDFLAGS ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES})

if (OPTION_BUILTIN_LIBCONFIG)
    list(APPEND LIBCONFIG_INCLUDES source/skeleton/libconfig)
    file(GLOB LIBCONFIG_SOURCES source/skeleton/libconfig/*.c)
else ()
    list(APPEND C2D_LDFLAGS config)
endif ()

if (OPTION_SDL2_GL OR OPTION_SDL2_GLES2 OR OPTION_SDL2_SOFT)
    pkg_search_module(SDL2 REQUIRED sdl2)
    list(APPEND C2D_INCLUDES ${SDL2_INCLUDE_DIRS})
    list(APPEND C2D_CFLAGS -D__SDL2__)
    list(APPEND C2D_LDFLAGS ${SDL2_LIBRARIES})
    file(GLOB SDL2_SRC source/platforms/sdl2/*.c*)
    list(APPEND C2D_SOURCES ${SDL2_SRC})
    if (OPTION_SDL2_GL OR OPTION_SDL2_GLES2)
        file(GLOB GL_SRC source/platforms/gl/*.c*)
        file(GLOB GL_SHADERS_SRC source/platforms/gl/shaders/*.c*)
        list(APPEND C2D_SOURCES ${GL_SRC} ${GL_SHADERS_SRC})
        list(APPEND C2D_CFLAGS -D__SDL2_GL__ -D__GL__)
    endif ()
    if (OPTION_SDL2_GL AND NOT PLATFORM_SWITCH)
        set(OpenGL_GL_PREFERENCE GLVND)
        find_package(OpenGL REQUIRED)
        list(APPEND C2D_INCLUDES ${OPENGL_INCLUDE_DIRS})
        list(APPEND C2D_LDFLAGS ${OPENGL_LIBRARIES})
    endif ()
    if (OPTION_SDL2_GLES2)
        list(APPEND C2D_CFLAGS -D__SDL2_GLES__)
        list(APPEND C2D_LDFLAGS GLESv2)
    endif ()
endif ()

if (OPTION_SDL1)
    pkg_search_module(SDL REQUIRED sdl)
    list(APPEND C2D_INCLUDES ${SDL_INCLUDE_DIRS})
    list(APPEND C2D_LDFLAGS ${SDL_LIBRARIES})
    list(APPEND C2D_CFLAGS -D__SDL1__)
    file(GLOB SDL_SRC source/platforms/sdl1/*.c*)
    list(APPEND C2D_SOURCES ${SDL_SRC})
endif ()

if (OPTION_RECALBOX_BUILDROOT)
    list(APPEND C2D_CFLAGS -D__RECALBOX__)
endif ()

# export tools
set(ZIP zip CACHE STRING "zip executable path")
####################
# COMMON STUFF
####################

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # VITA PLATORM
    #####################
    set(TITLE_ID CROSS0001)
    set(PLATFORM_INCLUDES
            source/platforms/posix
            source/platforms/psp2
            source/platforms/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SOURCES
            source/platforms/posix/posix_io.cpp
            source/platforms/psp2/*.c*
            source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/platforms/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            source/platforms/psp2/vita-shader-collection/lib/*.o
            )
    list(APPEND C2D_LDFLAGS
            SDL2 vita2d freetype
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub SceHid_stub
            jpeg png z m c
            )
    #png
    list(APPEND C2D_CFLAGS -Wl,-q -D__PSP2__ -DNO_KEYBOARD
            -Wno-uninitialized
            -ftree-vectorize -mword-relocations
            -fomit-frame-pointer -ffast-math
            -march=armv7-a -mtune=cortex-a9
            -mfpu=neon -mfloat-abi=hard)
    if (BUILD_DEBUG)
        list(APPEND C2D_CFLAGS -D__PSP2_DEBUG__)
    endif (BUILD_DEBUG)
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATORM
    #####################
    # shader
    #set(SHADER_AS picasso)
    #add_shbin_library(shaders.3ds src/3ds/vshader.v.pica)
    set(PLATFORM_INCLUDES
            source/platforms/3ds
            source/platforms/posix
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include
            #${DEVKITPRO}/portlibs/3ds/include/SDL2
            ${DEVKITPRO}/portlibs/3ds/include/freetype2
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SOURCES
            source/platforms/3ds/*.c*
            source/platforms/posix/*.c*
            #source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/platforms/3ds/vshaders.v.o
            source/platforms/3ds/render2d.shbin.o
            )
    list(APPEND C2D_LDFLAGS freetype png bz2 z citro3d ctru m)
    list(APPEND C2D_CFLAGS -O3 -DARM11 -D_3DS -D__3DS__ -D__CITRO3D__)
    #####################
    # SWITCH PLATORM
    #####################
elseif (PLATFORM_SWITCH)
    set(PLATFORM_INCLUDES
            source/platforms/posix
            source/platforms/switch
            ${DEVKITPRO}/portlibs/switch/include/freetype2
            )
    file(GLOB PLATFORM_SOURCES
            source/platforms/posix/posix_io.cpp
            source/platforms/posix/posix_clock.cpp
            source/platforms/switch/*.c*
            )
    list(APPEND C2D_CFLAGS
            -ffunction-sections -fomit-frame-pointer
            -D__SDL2_GL__ -D__GL__ -D__SWITCH__ -DNO_KEYBOARD
            -D_GLIBCXX_USE_C99_MATH_TR1 -D_LDBL_EQ_DBL
            -Wno-int-in-bool-context
            )
    list(APPEND C2D_LDFLAGS glad EGL glapi drm_nouveau nx freetype bz2 png z)
    ########################
    # LINUX PLATORM
    ########################
elseif (PLATFORM_LINUX)
    file(GLOB PLATFORM_SOURCES source/platforms/posix/*.c*)
    if (PLATFORM_WINDOWS)
        find_package(GLEW REQUIRED)
        # hum... static linking, not easy on mingw64 :)
        list(APPEND C2D_LDFLAGS ${GLEW_LIBRARIES} harfbuzz graphite2 rpcrt4 dwrite bz2 png z
                ole32 oleaut32 imm32 winmm version setupapi gdi32 opengl32 -static)
        list(APPEND C2D_CFLAGS -D__WINDOWS__ -DGLEW_STATIC=1)
        set(ZIP "/msys64/usr/bin/zip" CACHE STRING "zip executable path" FORCE)
    endif ()
endif ()

add_library(${PROJECT_NAME} STATIC ${C2D_SOURCES} ${LIBCONFIG_SOURCES} ${PLATFORM_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${C2D_INCLUDES} ${LIBCONFIG_INCLUDES} ${PLATFORM_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${C2D_LDFLAGS})
target_compile_options(${PROJECT_NAME} PUBLIC ${C2D_CFLAGS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcross2d.a DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/cross2d DESTINATION include)

#####################
# test executable
#####################
project(cross2d_test)
set(PROJECT_AUTHOR "Cpasjuste")
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
# ps vita
set(TITLE_ID "CROSS0001")
add_executable(${PROJECT_NAME} test/main.cpp)
target_link_libraries(${PROJECT_NAME} cross2d)

#############
# targets
#############
include(Targets.cmake)

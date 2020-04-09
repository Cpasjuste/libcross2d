cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
option(PLATFORM_LINUX "Linux platform target" OFF)
option(PLATFORM_WINDOWS "Windows platform target (msys/mingw64)" OFF)
option(PLATFORM_RPI3 "RIP3 platform target" OFF)
option(PLATFORM_SWITCH "Nintendo Switch platform target" OFF)
option(PLATFORM_3DS "Nintendo 3DS platform target" OFF)
option(PLATFORM_VITA "Sony PS Vita platform target" OFF)
option(PLATFORM_DREAMCAST "Sega Dreamcast platform target" OFF)
####################
# TARGET PLATFORM
####################

####################
# BUILD OPTIONS
####################
option(OPTION_SDL1 "SDL1 support (software, slow, unsupported)" OFF)
option(OPTION_SDL2 "SDL2 support" OFF)
#option(OPTION_SDL2_SOFT "SDL2 support (software, slow)" OFF)
option(OPTION_RENDER_GL1 "OpenGL 1.2 rendering" OFF)
option(OPTION_RENDER_GL2 "OpenGL 3.3 rendering" OFF)
option(OPTION_RENDER_GLES2 "OpenGLES 2.0 rendering" OFF)
option(OPTION_BUILTIN_LIBCONFIG "Build libconfig from sources" OFF)
option(OPTION_FREEPLAY "Build with FreePlay (CM3/ZERO) support" OFF)
option(OPTION_RECALBOX_BUILDROOT "Build from recalbox buildroot" OFF)
set(OPTION_SCREEN_WIDTH 1280 CACHE STRING "Custom screen width")
set(OPTION_SCREEN_HEIGHT 720 CACHE STRING "Custom screen height")
####################
# BUILD OPTIONS
####################

####################
# SANITY CHECKS
####################
if (PLATFORM_LINUX OR PLATFORM_WINDOWS OR PLATFORM_SWITCH)
    if (NOT (OPTION_SDL1 OR OPTION_RENDER_GL1 OR OPTION_RENDER_GLES2))
        message(STATUS "C2D: SDL2 OpenGL 3.3 support enabled")
        set(OPTION_SDL2 ON CACHE BOOL "SDL2 support" FORCE)
        set(OPTION_RENDER_GL2 ON CACHE BOOL "OpenGL 3.3 rendering" FORCE)
    endif ()
endif ()

if (PLATFORM_RPI3)
    message(STATUS "C2D: SDL2 OpenGLES 2.0 support enabled")
    set(OPTION_SDL2 ON CACHE BOOL "SDL2 support" FORCE)
    set(OPTION_RENDER_GLES2 ON CACHE BOOL "OpenGLES 2.0 rendering" FORCE)
endif ()

if (PLATFORM_DREAMCAST)
    message(STATUS "C2D: DREAMCAST OpenGL 1.2 support enabled")
    set(OPTION_RENDER_GL1 ON CACHE BOOL "OpenGL 1.2 rendering" FORCE)
    set(OPTION_SCREEN_WIDTH 640 CACHE STRING "Custom screen width" FORCE)
    set(OPTION_SCREEN_HEIGHT 480 CACHE STRING "Custom screen height" FORCE)
endif ()

if (OPTION_RENDER_GL1 AND NOT PLATFORM_DREAMCAST)
    message(STATUS "C2D: SDL1 with OpenGL 1.2 support enabled")
    set(OPTION_SDL1 ON CACHE BOOL "OpenGL 1.2 rendering" FORCE)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()
####################
# SANITY CHECKS
####################

# setup target toolchain
include(cmake/toolchain.cmake)

project(cross2d)

####################
# COMMON STUFF
####################
file(GLOB C2D_SOURCES
        source/widgets/*.c*
        source/skeleton/*.c*
        source/skeleton/sfml/*.c*)
if (NOT PLATFORM_DREAMCAST AND NOT PLATFORM_3DS)
    include(FindPkgConfig)
    pkg_search_module(ZLIB REQUIRED zlib)
    pkg_search_module(FREETYPE REQUIRED freetype2)
endif ()
set(C2D_INCLUDES include ${FREETYPE_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(C2D_CFLAGS -Wall -Wno-narrowing -DLIBCONFIG_STATIC)
set(C2D_LDFLAGS ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES})

if (BUILD_DEBUG)
    list(APPEND C2D_CFLAGS -g -O0)
else ()
    if (NOT PLATFORM_DREAMCAST)
        list(APPEND C2D_CFLAGS -g -O3)
    endif ()
endif ()

if (OPTION_BUILTIN_LIBCONFIG)
    list(APPEND LIBCONFIG_INCLUDES source/skeleton/libconfig)
    file(GLOB LIBCONFIG_SOURCES source/skeleton/libconfig/*.c)
else ()
    list(APPEND C2D_LDFLAGS -lconfig)
endif ()

if (OPTION_SDL1)
    pkg_search_module(SDL REQUIRED sdl)
    list(APPEND C2D_INCLUDES ${SDL_INCLUDE_DIRS})
    list(APPEND C2D_LDFLAGS ${SDL_LIBRARIES})
    list(APPEND C2D_CFLAGS -D__SDL1__)
    file(GLOB SDL_SRC source/platforms/sdl1/*.c*)
    list(APPEND C2D_SOURCES ${SDL_SRC})
endif ()

if (OPTION_SDL2)
    pkg_search_module(SDL2 REQUIRED sdl2)
    list(APPEND C2D_INCLUDES ${SDL2_INCLUDE_DIRS})
    list(APPEND C2D_CFLAGS -D__SDL2__)
    list(APPEND C2D_LDFLAGS ${SDL2_LIBRARIES})
    file(GLOB SDL2_SRC source/platforms/sdl2/*.c*)
    list(APPEND C2D_SOURCES ${SDL2_SRC})
endif ()

if (OPTION_RENDER_GL2 OR OPTION_RENDER_GLES2)
    file(GLOB GL_SRC source/platforms/gl2/*.c*)
    file(GLOB GL_SHADERS_SRC source/platforms/gl2/shaders/*.c*)
    list(APPEND C2D_SOURCES ${GL_SRC} ${GL_SHADERS_SRC})
    list(APPEND C2D_CFLAGS -D__GL2__)
    if (OPTION_RENDER_GLES2)
        list(APPEND C2D_CFLAGS -D__GLES2__)
        list(APPEND C2D_LDFLAGS GLESv2)
    endif ()
    if (NOT PLATFORM_SWITCH)
        set(OpenGL_GL_PREFERENCE GLVND)
        find_package(OpenGL REQUIRED)
        list(APPEND C2D_INCLUDES ${OPENGL_INCLUDE_DIRS})
        list(APPEND C2D_LDFLAGS ${OPENGL_LIBRARIES})
    endif ()
endif ()

if (OPTION_RENDER_GL1)
    file(GLOB GL_SRC source/platforms/gl1/*.c*)
    list(APPEND C2D_SOURCES ${GL_SRC})
    list(APPEND C2D_CFLAGS -D__GL1__)
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
    set(OPTION_SCREEN_WIDTH 960 CACHE STRING "" FORCE)
    set(OPTION_SCREEN_HEIGHT 544 CACHE STRING "" FORCE)
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
    list(APPEND C2D_CFLAGS -D__PSP2__ -DNO_KEYBOARD
            -Wno-uninitialized -ftree-vectorize -mword-relocations
            -fomit-frame-pointer -ffast-math)
    if (BUILD_DEBUG)
        list(APPEND C2D_CFLAGS -D__PSP2_DEBUG__)
    endif (BUILD_DEBUG)
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATORM
    #####################
    set(OPTION_SCREEN_WIDTH 400 CACHE STRING "" FORCE)
    set(OPTION_SCREEN_HEIGHT 240 CACHE STRING "" FORCE)
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
    list(APPEND C2D_LDFLAGS freetype png bz2 z citro2d citro3d ctru m)
    list(APPEND C2D_CFLAGS -DARM11 -D_3DS -D__3DS__ -D__CITRO3D__)
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
            -D__SDL2_GL__ -D__GL2__ -D__SWITCH__ -DNO_KEYBOARD
            -D_GLIBCXX_USE_C99_MATH_TR1 -D_LDBL_EQ_DBL
            -Wno-int-in-bool-context
            )
    list(APPEND C2D_LDFLAGS glad EGL glapi drm_nouveau nx freetype bz2 png z)
    ########################
    # DREAMCAST PLATORM
    ########################
elseif (PLATFORM_DREAMCAST)
    enable_language(ASM)
    file(GLOB_RECURSE PLATFORM_SOURCES source/platforms/dreamcast/*.c*)
    set(PLATFORM_INCLUDES
            source/platforms/dreamcast
            $ENV{KOS_BASE}/../kos-ports/include/freetype
            $ENV{KOS_BASE}/../kos-ports/include/libconfig)
    list(APPEND C2D_CFLAGS -D__DREAMCAST__ -DNO_KEYBOARD)
    list(APPEND C2D_LDFLAGS c kallisti c GLdc freetype z m) # kosfat
    ########################
    # LINUX PLATORM
    ########################
elseif (PLATFORM_LINUX)
    list(APPEND C2D_CFLAGS -D__PLATFORM_LINUX__)
    file(GLOB PLATFORM_SOURCES source/platforms/posix/*.c*)
    add_subdirectory(source/platforms/gl2/glad)
    list(APPEND PLATFORM_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/source/platforms/gl/glad/include)
    list(APPEND C2D_LDFLAGS glad dl)
    if (PLATFORM_RPI3)
        if (OPTION_FREEPLAY)
            list(APPEND C2D_CFLAGS -D__FREEPLAY__)
            list(APPEND C2D_LDFLAGS fbcp)
        endif ()
        list(APPEND C2D_CFLAGS -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -mlittle-endian -munaligned-access)
        list(APPEND C2D_LDFLAGS bcm_host vchostif pthread dl)
    endif ()
    if (PLATFORM_WINDOWS)
        find_package(GLEW REQUIRED)
        # hum... static linking, not easy on mingw64 :)
        list(APPEND C2D_LDFLAGS glew32 freetype harfbuzz graphite2 rpcrt4 dwrite bz2 png z
                ole32 oleaut32 imm32 winmm version setupapi gdi32 opengl32 -static)
        list(APPEND C2D_CFLAGS -DGLEW_STATIC=1)
        set(ZIP "/usr/bin/zip" CACHE STRING "zip executable path" FORCE)
    endif ()
endif ()

list(APPEND C2D_CFLAGS -DC2D_SCREEN_WIDTH=${OPTION_SCREEN_WIDTH} -DC2D_SCREEN_HEIGHT=${OPTION_SCREEN_HEIGHT})

add_library(${PROJECT_NAME} STATIC ${C2D_SOURCES} ${LIBCONFIG_SOURCES} ${PLATFORM_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${C2D_INCLUDES} ${LIBCONFIG_INCLUDES} ${PLATFORM_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${C2D_LDFLAGS})
target_compile_options(${PROJECT_NAME} PUBLIC ${C2D_CFLAGS})

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
include(cmake/targets.cmake)

#############
# install
#############
install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        PERMISSIONS OWNER_EXECUTE OWNER_READ
        DESTINATION bin
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcross2d.a DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/cross2d DESTINATION include)

cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
option(PLATFORM_LINUX "Linux platform target" ON)
option(PLATFORM_WINDOWS "Windows platform target (msys/mingw64)" OFF)
option(PLATFORM_RPI3_64 "RIP3 (64 bits) platform target" OFF)
option(PLATFORM_SWITCH "Nintendo Switch platform target" OFF)
option(PLATFORM_3DS "Nintendo 3DS platform target" OFF)
option(PLATFORM_VITA "Sony PS Vita platform target" OFF)
option(PLATFORM_LDK "LDK Handheld platform target (mips, LDK, RS-97...)" OFF)
#option(PLATFORM_PS3 "PS3 platform target" OFF) // TODO: fix for latest changes

####################
# PLATFORM OPTIONS
####################
option(OPTION_SDL2_GL "SDL2 support (OpenGL 3.3)" OFF)
option(OPTION_SDL2_GLES2 "SDL2 support (OpenGLES 2.0)" OFF)
option(OPTION_SDL2_SOFT "SDL2 support (software renderer)" OFF)

if (PLATFORM_LINUX OR PLATFORM_WINDOWS OR PLATFORM_SWITCH)
    if (NOT OPTION_SDL2_GL AND NOT OPTION_SDL2_GLES2 AND NOT OPTION_SDL2_SOFT)
        set(OPTION_SDL2_GL ON CACHE BOOL "SDL2 support (OpenGL 3.3)")
    endif ()
endif ()

if (PLATFORM_RPI3_64)
    set(OPTION_SDL2_GLES2 ON CACHE BOOL "SDL2 support (OpenGL 3.3)")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()

# setup toolchain
# set(CMAKE_TOOLCHAIN_FILE Toolchain.cmake)
include(Toolchain.cmake)

project(cross2d)

# export tools
set(ZIP zip CACHE STRING "zip executable path")

# flags
set(FLAGS -Wall -DLIBCONFIG_STATIC)

##############
# CROSS2D
##############
set(CROSS2D_DIR include)

file(GLOB CROSS2D_SRC
        source/widgets/*.c*
        source/skeleton/*.c*
        source/skeleton/sfml/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # VITA PLATORM
    #####################
    set(TITLE_ID CROSS0001)
    set(PLATFORM_DIRS
            source/skeleton/libconfig
            source/platforms/posix
            source/platforms/psp2
            source/platforms/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/skeleton/libconfig/*.c
            source/platforms/posix/posix_io.cpp
            source/platforms/psp2/*.c*
            source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/platforms/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            source/platforms/psp2/vita-shader-collection/lib/*.o
            )
    set(PLATFORM_LIBS
            SDL2 vita2d freetype
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub SceHid_stub
            jpeg png z m c
            )
    #png
    list(APPEND FLAGS -Wl,-q -D__PSP2__
            -Wno-uninitialized
            -ftree-vectorize -mword-relocations
            -fomit-frame-pointer -ffast-math
            -march=armv7-a -mtune=cortex-a9
            -mfpu=neon -mfloat-abi=hard)
    if (BUILD_DEBUG)
        list(APPEND FLAGS -D__PSP2_DEBUG__)
    endif (BUILD_DEBUG)
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATORM
    #####################
    # shader
    #set(SHADER_AS picasso)
    #add_shbin_library(shaders.3ds src/3ds/vshader.v.pica)
    set(PLATFORM_DIRS
            source/platforms/3ds
            source/platforms/posix
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include
            #${DEVKITPRO}/portlibs/3ds/include/SDL2
            ${DEVKITPRO}/portlibs/3ds/include/freetype2
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SRC
            source/skeleton/libconfig/*.c
            source/platforms/3ds/*.c*
            source/platforms/posix/*.c*
            #source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/platforms/3ds/vshaders.v.o
            source/platforms/3ds/render2d.shbin.o
            )

    set(PLATFORM_LIBS freetype png bz2 z citro3d ctru m)
    list(APPEND FLAGS -O3 -DARM11 -D_3DS -D__3DS__ -D__CITRO3D__)
    #####################
    # SWITCH PLATORM
    #####################
elseif (PLATFORM_SWITCH)
    set(PLATFORM_DIRS
            source/platforms/gl
            source/platforms/sdl2
            source/platforms/posix
            source/platforms/switch
            ${DEVKITPRO}/portlibs/switch/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/platforms/gl/*.c*
            source/platforms/gl/shaders/*.c*
            source/platforms/sdl2/*.c*
            source/platforms/posix/posix_io.cpp
            source/platforms/posix/posix_clock.cpp
            source/platforms/switch/*.c*
            )
    list(APPEND FLAGS
            -ffunction-sections -fomit-frame-pointer
            -D__SDL2_GL__ -D__GL__ -D__SWITCH__
            -D_GLIBCXX_USE_C99_MATH_TR1 -D_LDBL_EQ_DBL
            -Wno-int-in-bool-context
            )
    set(PLATFORM_LIBS
            SDL2 glad EGL glapi drm_nouveau nx
            freetype bz2 png config z
            )
    ########################
    # LINUX PLATORM (SDL2)
    ########################
elseif (PLATFORM_LINUX)
    if (NOT BUILD_LINUX_SOFT AND NOT BUILD_SDL2_GLES)
        set(OpenGL_GL_PREFERENCE GLVND)
        find_package(OpenGL REQUIRED)
    endif ()
    if (NOT BUILD_MIPS AND NOT BUILD_SDL2_GLES)
        find_package(SDL2 REQUIRED)
    endif ()
    find_package(ZLIB REQUIRED)
    find_package(Freetype REQUIRED)
    set(PLATFORM_DIRS
            source/platforms/gl
            source/platforms/posix
            ${OPENGL_INCLUDE_DIRS}
            ${SDL2_INCLUDE_DIRS}
            ${FREETYPE_INCLUDE_DIRS}
            ${ZLIB_INCLUDE_DIRS})
    file(GLOB PLATFORM_SRC
            source/platforms/gl/*.c*
            source/platforms/gl/shaders/*.c*
            source/platforms/sdl2/*.c*
            source/platforms/posix/*.c*)
    set(PLATFORM_LIBS ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES} config)
    if (BUILD_LINUX_SOFT)
        list(APPEND FLAGS -D__SDL2__)
    elseif (OPTION_SDL2_GLES2)
        list(APPEND FLAGS -D__SDL2__ -D__SDL2_GL__ -D__SDL2_GLES__ -D__GL__)
    else ()
        list(APPEND FLAGS -D__SDL2__ -D__SDL2_GL__ -D__GL__)
    endif ()
    if (PLATFORM_LDK)
        list(APPEND PLATFORM_LIBS SDL2)
    elseif (BUILD_SDL2_GLES)
        list(APPEND PLATFORM_LIBS SDL2 GLESv2 OSMesa)
    endif ()
    if (PLATFORM_WINDOWS)
        find_package(GLEW REQUIRED)
        # hum... static linking, not easy on mingw64 :)
        list(APPEND PLATFORM_LIBS ${GLEW_LIBRARIES} harfbuzz graphite2 rpcrt4 dwrite bz2 png z
                ole32 oleaut32 imm32 winmm version setupapi gdi32 opengl32 -static)
        list(APPEND FLAGS -D__WINDOWS__ -DGLEW_STATIC=1)
        set(ZIP "/msys64/usr/bin/zip" CACHE STRING "zip executable path" FORCE)
    endif ()
endif ()

add_library(${PROJECT_NAME} ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CROSS2D_DIR} ${PLATFORM_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PLATFORM_LIBS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FLAGS} -Wno-narrowing)

#install(FILES ${CMAKE_BINARY_DIR}/libcross2d.a DESTINATION lib)
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/cross2d DESTINATION include)

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

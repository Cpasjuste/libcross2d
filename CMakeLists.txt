cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support (hardware rendering, fast)" OFF)
option(BUILD_SDL1 "Build with SDL1 support (software, osmesa) support" OFF)
option(BUILD_SDL1_GL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)" OFF)
option(BUILD_SDL2 "Build with SDL2 support (software)" OFF)
option(BUILD_SDL2_GL "Build with SDL2 support (hardware rendering, fast, needs BUILD_SDL2=ON)" OFF)
option(BUILD_PSP2 "Build with PSP2 support (hardware rendering, fast)" OFF)
option(BUILD_PS3 "Build with PS3 support (hardware rendering, fast)" OFF)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_NX "Build with NX (SWITCH) support" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()

# setup toolchain
include(Toolchain.cmake)

project(cross2d)

# flags
set(FLAGS -Wall)

##############
# CROSS2D
##############
set(CROSS2D_DIR
        include)

file(GLOB CROSS2D_SRC
        source/widgets/*.c*
        source/skeleton/*.c*
        source/skeleton/sfml/*.c*)

#####################
# PLATORM SPECIFIC
#####################
#####################
# SFML PLATORM
#####################
if (BUILD_SFML)
    set(PLATFORM_DIRS
            source/platforms/sfml
            source/platforms/posix
            )
    file(GLOB PLATFORM_SRC
            source/platforms/sfml/*.c*
            source/platforms/posix/*.c*
            source/platforms/sdl2/sdl2_audio.cpp #TODO: get rid of sdl2, use sdl2 audio for now
            )
    list(APPEND FLAGS
            -D__SFML__
            -I/usr/include/freetype2
            )
    set(PLATFORM_LIBS
            cross2d SDL2
            sfml-graphics sfml-window sfml-system
            GL freetype png z)
    #####################
    # PSP2 PLATORM
    #####################
elseif (BUILD_PSP2)
    set(PLATFORM_DIRS
            source/platforms/psp2
            source/platforms/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/platforms/psp2/*.c*
            source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/platforms/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            source/platforms/psp2/vita-shader-collection/lib/*.o
            )
    set(PLATFORM_LIBS
            cross2d SDL2 vita2d freetype
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub SceHid_stub
            png jpeg z m c
            )
    list(APPEND FLAGS -Wl,-q -D__PSP2__
            -Wno-uninitialized
            -ftree-vectorize -mword-relocations
            -fomit-frame-pointer -ffast-math
            -march=armv7-a -mtune=cortex-a9
            -mfpu=neon -mfloat-abi=hard)
    if (BUILD_DEBUG)
        list(APPEND FLAGS -D__PSP2_DEBUG__)
    endif (BUILD_DEBUG)
    #####################
    # PS3 PLATORM
    #####################
elseif (BUILD_PS3)
    set(CMAKE_C_FLAGS_RELEASE "-O2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    set(PLATFORM_DIRS
            source/platforms/ps3
            source/platforms/ps3/tiny3d
            include/platforms/ps3/tiny3d
            source/platforms/posix
            ${PSL1GHT}/portlibs/ppu/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/platforms/ps3/*.c*
            source/platforms/ps3/tiny3d/*.c*
            source/platforms/ps3/tiny3d/*.S*
            source/platforms/posix/*.c*
            )
    set(PLATFORM_LIBS
            ${PSL1GHT}/portlibs/ppu/lib/libfreetype.a
            ${PSL1GHT}/portlibs/ppu/lib/libpng.a
            ${PSL1GHT}/portlibs/ppu/lib/libtiff.a
            ${PSL1GHT}/portlibs/ppu/lib/libjpeg.a
            ${PSL1GHT}/ppu/lib/libgcm_sys.a
            ${PSL1GHT}/ppu/lib/librsx.a
            ${PSL1GHT}/ppu/lib/libsysutil.a
            ${PSL1GHT}/ppu/lib/libsysmodule.a
            ${PSL1GHT}/ppu/lib/libpngdec.a
            ${PSL1GHT}/ppu/lib/libio.a
            ${PSL1GHT}/ppu/lib/libaudio.a
            ${PSL1GHT}/portlibs/ppu/lib/libz.a
            )
    list(APPEND FLAGS -D__PS3__)
    #####################
    # 3DS PLATORM
    #####################
elseif (BUILD_3DS)
    # shader
    #set(SHADER_AS picasso)
    #add_shbin_library(shaders.3ds src/3ds/vshader.v.pica)
    set(PLATFORM_DIRS
            source/platforms/3ds
            source/platforms/posix
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SRC
            source/platforms/3ds/*.c*
            source/platforms/posix/*.c*
            source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/platforms/3ds/vshaders.v.o
            )

    set(PLATFORM_LIBS
            cross2d
            ${DEVKITPRO}/portlibs/3ds/lib/libSDL2.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libz.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
    list(APPEND FLAGS -O3 -DARM11 -D_3DS -D__3DS__
            -D__CITRO3D__
            -ffunction-sections -fomit-frame-pointer
            )
    #####################
    # NX PLATORM
    #####################
elseif (BUILD_NX)
    add_subdirectory(source/platforms/nx/nxnetprint)
    set(PLATFORM_DIRS
            source/platforms/nx
            source/platforms/nx/nxnetprint/include
            source/platforms/posix
            ${DEVKITPRO}/libnx/include
            ${DEVKITPRO}/portlibs/switch/include
            ${DEVKITPRO}/portlibs/switch/include/SDL2
            ${DEVKITPRO}/portlibs/switch/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/platforms/nx/*.c*
            source/platforms/sdl2/sdl2_input.cpp
            source/platforms/sdl2/sdl2_audio.cpp
            source/platforms/sdl2/sdl2_renderer.cpp
            source/platforms/sdl2/sdl2_texture.cpp
            source/platforms/posix/posix_io.cpp
            )
    list(APPEND FLAGS -D__NX__ -D__SWITCH__
            -ffunction-sections -fomit-frame-pointer
            )
    set(PLATFORM_LIBS
            cross2d nxnetprint
            ${DEVKITPRO}/portlibs/switch/lib/libSDL2.a
            ${DEVKITPRO}/libnx/lib/libnx.a
            ${DEVKITPRO}/portlibs/switch/lib/libfreetype.a
            ${DEVKITPRO}/portlibs/switch/lib/libpng.a
            ${DEVKITPRO}/portlibs/switch/lib/libbz2.a
            ${DEVKITPRO}/portlibs/switch/lib/libz.a)
    #####################
    # SDL2 PLATORM
    #####################
elseif (BUILD_SDL2)
    set(PLATFORM_DIRS
            source/platforms/sdl2
            source/platforms/posix
            /usr/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/platforms/sdl2/*.c*
            source/platforms/posix/*.c*
            )
    list(APPEND FLAGS -D__SDL2__)
    if (BUILD_SDL2_GL)
        list(APPEND PLATFORM_DIRS source/platforms/gl)
        list(APPEND PLATFORM_SRC
                source/platforms/gl/gl_renderer.cpp
                source/platforms/gl/gl_texture.cpp)
        list(APPEND FLAGS -D__SDL2_GL__ -D__GL__)
        set(PLATFORM_LIBS cross2d GL SDL2 freetype png z)
    else ()
        set(PLATFORM_LIBS cross2d SDL2 freetype png z)
    endif (BUILD_SDL2_GL)
    #####################
    # SDL1 PLATORM
    #####################
elseif (BUILD_SDL1)
    set(PLATFORM_DIRS
            source/platforms/sdl1
            source/platforms/gl
            source/platforms/posix
            /usr/include/freetype2)
    file(GLOB PLATFORM_SRC
            source/platforms/sdl1/*.c*
            source/platforms/gl/*.c*
            source/platforms/posix/*.c*)
    list(APPEND FLAGS -D__SDL1__ -D__GL__)
    if (BUILD_SDL1_GL)
        list(APPEND FLAGS -D__SDL1_GL__)
        set(PLATFORM_LIBS cross2d GL SDL freetype png z)
    else ()
        set(PLATFORM_LIBS cross2d OSMesa pthread SDL freetype png z)
    endif (BUILD_SDL1_GL)
endif (BUILD_SFML)

add_library(cross2d ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(cross2d PUBLIC ${CROSS2D_DIR} ${PLATFORM_DIRS})
target_compile_options(cross2d PUBLIC ${FLAGS})

install(FILES ${CMAKE_BINARY_DIR}/libcross2d.a DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION include/cross2d)

#####################
# test executable
#####################
add_executable(cross2d.elf test/main.cpp)
target_compile_options(cross2d.elf PRIVATE ${FLAGS})
target_link_libraries(cross2d.elf ${PLATFORM_LIBS})

#####################
# PS3 target
#####################
if (BUILD_PS3)
    add_custom_target(cross2d.ps3
            DEPENDS cross2d.elf
            COMMAND ${PSL1GHT}/bin/sprxlinker cross2d.elf)
    add_dependencies(cross2d.ps3 cross2d.elf)
endif (BUILD_PS3)

#####################
# PSP2 (vita) target
#####################
if (BUILD_PSP2)
    vita_create_self(cross2d.self cross2d.elf)
    vita_create_vpk(cross2d.vpk "CROSS2D00" cross2d.self
            VERSION "01.00" NAME "CROSS2D"
            FILE ${CMAKE_CURRENT_SOURCE_DIR}/res/default.ttf default.ttf
            FILE ${CMAKE_CURRENT_SOURCE_DIR}/res/title.png title.png)
endif (BUILD_PSP2)

#####################
# CTR (3DS) target
#####################
if (BUILD_3DS)
    set_target_properties(cross2d.elf PROPERTIES LINK_FLAGS "-specs=3dsx.specs")
    add_3dsx_target(cross2d.elf NO_SMDH)
endif (BUILD_3DS)

#####################
# NX (SWITCH) target
#####################
if (BUILD_NX)
    set_target_properties(cross2d.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(cross2d.nro
            DEPENDS cross2d.elf
            COMMAND elf2nro cross2d.elf cross2d.nro)
endif (BUILD_NX)

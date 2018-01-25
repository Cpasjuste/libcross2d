cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")
enable_language(ASM)

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support (hardware rendering, fast)" OFF)
option(BUILD_SDL1 "Build with SDL1 support (software, osmesa) support" OFF)
option(BUILD_SDL1_GL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)" OFF)
option(BUILD_SDL1_TINYGL "Build with SDL1 support (software rendering, slow, needs BUILD_SDL1=ON)" OFF)
option(BUILD_SDL2 "Build with SDL2 support (software, osmesa) support" OFF)
option(BUILD_SDL2_GL "Build with SDL2 support (hardware rendering, fast, needs BUILD_SDL2=ON)" OFF)
option(BUILD_PSP2 "Build with PSP2 support (hardware rendering, fast)" OFF)
option(BUILD_PSP2_GL "Build with PSP2 TinyGL support (testing, software rendering, slow, needs BUILD_PSP2=ON)" OFF)
option(BUILD_PSP2_OSMESA "Build with PSP2 OSMesa support (testing, software rendering, slow, needs BUILD_PSP2=ON)" OFF)
option(BUILD_PS3 "Build with PS3 support (hardware rendering, fast)" OFF)
#option(BUILD_3DS "Build with 3DS support" OFF)
#option(BUILD_RPI "Build with RPI support" OFF)
option(BUILD_NX "Build with NX (SWITCH) support" OFF)
#option(BUILD_SWITCH_TRANSISTOR "Build with NX (SWITCH) support" ON)

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
        source/skeleton/*.c*
        source/skeleton/c2d_sfml/*.c*)

#####################
# PLATORM SPECIFIC
#####################
#####################
# SFML PLATORM
#####################
if (BUILD_SFML)
    set(PLATFORM_DIRS
            source/sfml
            source/posix
            )
    file(GLOB PLATFORM_SRC
            source/sfml/*.c*
            source/posix/*.c*
            source/sdl2/sdl2_audio.cpp #TODO: get rid of sdl2, use sdl2 audio for now
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
            source/psp2
            #source/psp2/libvita2d/include
            source/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            #source/psp2/*.c*
            source/psp2/psp2_shaders.cpp
            #source/psp2/libvita2d/source/*.c
            source/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            #source/psp2/libvita2d/shader/compiled/*.o
            #source/psp2/vita-shader-collection/lib/*.o
            )
    set(PLATFORM_LIBS
            cross2d SDL2 vita2d freetype
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub
            png jpeg z m c
            )
    if (BUILD_PSP2_GL)
        list(APPEND PLATFORM_DIRS include/skeleton/TinyGL)
        file(GLOB GL_SRC source/skeleton/TinyGL/*.c)
        list(APPEND PLATFORM_SRC
                ${GL_SRC}
                source/psp2_tinygl/psp2gl_renderer.cpp)
        list(APPEND FLAGS -D__GL__ -D__PSP2_GL__ -D__TINYGL__)
    elseif (BUILD_PSP2_OSMESA)
        list(APPEND PLATFORM_SRC
                source/psp2_osmesa/psp2osmesa_renderer.cpp)
        list(APPEND FLAGS -D__PSP2_OSMESA__)
        list(APPEND PLATFORM_LIBS OSMesa pthread)
    else ()
        list(APPEND PLATFORM_SRC
                source/psp2/psp2_renderer.cpp
                source/psp2/psp2_texture.cpp)
    endif (BUILD_PSP2_GL)
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
            source/ps3
            source/ps3/tiny3d
            include/ps3/tiny3d
            ${PSL1GHT}/portlibs/ppu/include/freetype2
            source/posix
            )
    file(GLOB PLATFORM_SRC
            source/ps3/*.c*
            source/ps3/tiny3d/*.c*
            source/ps3/tiny3d/*.S*
            source/posix/*.c*
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
            source/3ds
            source/posix
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SRC
            source/3ds/*.c*
            source/posix/*.c*
            source/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/3ds/vshaders.v.o
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
    set(PLATFORM_DIRS
            source/nx
            source/posix
            source/skeleton/pTinyGL/include
            ${DEVKITPRO}/libnx/include
            ${DEVKITPRO}/portlibs/armv8-a/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/nx/*.c*
            source/posix/*.c*
            source/skeleton/pTinyGL/source/pgl_nx.c
            source/skeleton/pTinyGL/source/api/*.c*
            )
    list(APPEND FLAGS -D__NX__ -D__GL__ -D__TINYGL__
            -ffunction-sections -fomit-frame-pointer
            )
    set(PLATFORM_LIBS cross2d
            ${DEVKITPRO}/libnx/lib/libnx.a
            ${DEVKITPRO}/portlibs/armv8-a/lib/libfreetype.a
            ${DEVKITPRO}/portlibs/armv8-a/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv8-a/lib/libz.a)
elseif (BUILD_SWITCH_TRANSISTOR)
    set(PLATFORM_DIRS
            )
    file(GLOB PLATFORM_SRC
            )
    list(APPEND FLAGS -D__TRANSISTOR__
            -ffunction-sections -fomit-frame-pointer
            )
    #####################
    # SDL2 PLATORM
    #####################
elseif (BUILD_SDL2)
    set(PLATFORM_DIRS
            source/sdl2
            source/posix
            /usr/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/sdl2/*.c*
            source/posix/*.c*
            )
    list(APPEND FLAGS -D__SDL2__ -D__GL__)
    if (BUILD_SDL2_GL)
        list(APPEND FLAGS -D__SDL2_GL__)
        set(PLATFORM_LIBS cross2d GL SDL2 freetype png z)
    else ()
        set(PLATFORM_LIBS cross2d OSMesa pthread SDL2 freetype png z)
    endif (BUILD_SDL2_GL)
    #####################
    # SDL1 PLATORM
    #####################
elseif (BUILD_SDL1)
    set(PLATFORM_DIRS
            source/sdl1
            source/posix
            /usr/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            source/sdl1/*.c*
            source/posix/*.c*
            )
    list(APPEND FLAGS -D__SDL1__ -D__GL__)
    if (BUILD_SDL1_GL)
        list(APPEND FLAGS -D__SDL1_GL__)
        set(PLATFORM_LIBS cross2d GL SDL freetype png z)
    elseif (BUILD_SDL1_TINYGL)
        list(APPEND PLATFORM_DIRS source/skeleton/pTinyGL/include)
        file(GLOB TINYGL_SRC
                source/skeleton/pTinyGL/source/pgl_sdl1.c
                source/skeleton/pTinyGL/source/api/*.c*
                )
        list(APPEND PLATFORM_SRC ${TINYGL_SRC})
        list(APPEND FLAGS -D__TINYGL__)
        set(PLATFORM_LIBS cross2d SDL freetype png z)
    else ()
        set(PLATFORM_LIBS cross2d OSMesa pthread SDL freetype png z)
    endif (BUILD_SDL1_GL)
endif (BUILD_SFML)


add_library(${PROJECT_NAME} ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CROSS2D_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PLATFORM_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

#####################
# test executable
#####################
add_executable(${PROJECT_NAME}.elf test/main.cpp)
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.elf ${PLATFORM_LIBS})

#####################
# PS3 target
#####################
if (BUILD_PS3)
    add_custom_target(${PROJECT_NAME}.ps3
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND ${PSL1GHT}/bin/sprxlinker ${PROJECT_NAME}.elf)
    add_dependencies(${PROJECT_NAME}.ps3 ${PROJECT_NAME}.elf)
endif (BUILD_PS3)

#####################
# PSP2 (vita) target
#####################
if (BUILD_PSP2)
    add_custom_target(${PROJECT_NAME}.vpk
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
            COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
            COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf vpk/eboot.bin
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLE_ID} "${PROJECT_NAME}" vpk/sce_sys/param.sfo
            COMMAND cp -r ../res/* vpk/
            COMMAND cd vpk && zip -r ../${PROJECT_NAME}-${BUILD_DATE}.vpk . && cd ..
            )
    add_dependencies(${PROJECT_NAME}.vpk ${PROJECT_NAME}.elf)
endif (BUILD_PSP2)

#####################
# CTR (3DS) target
#####################
if (BUILD_3DS)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=3dsx.specs")
    add_3dsx_target(${PROJECT_NAME}.elf NO_SMDH)
endif (BUILD_3DS)

#####################
# NX (SWITCH) target
#####################
if (BUILD_NX)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro)
endif (BUILD_NX)

cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")
enable_language(ASM)

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support" OFF)
option(BUILD_SDL2 "Build with SDL2 support" ON)
option(BUILD_SDL1 "Build with SDL1 support" OFF)
option(BUILD_PSP2 "Build with PSP2 support" OFF)
option(BUILD_PS3 "Build with PS3 support" OFF)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_RPI "Build with RPI support" OFF)
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
set(CROSS2D_DIR include)
set(CROSS2D_SRC
        source/skeleton/widget.cpp
        source/skeleton/renderer.cpp
        source/skeleton/rectangle.cpp
        #source/skeleton/line.cpp
        source/skeleton/c2d_sfml/Shape.cpp
        source/skeleton/c2d_sfml/Color.cpp
        source/skeleton/c2d_sfml/Transform.cpp
        source/skeleton/c2d_sfml/Transformable.cpp
        source/skeleton/c2d_sfml/Vertex.cpp
        source/skeleton/c2d_sfml/VertexArray.cpp
        source/skeleton/c2d_sfml/RectangleShape.cpp
        )
#file(GLOB CROSS2D_SRC src/skeleton/*.c*)

#####################
# PLATORM SPECIFIC
#####################
#####################
# PSP2 PLATORM
#####################
if (BUILD_PSP2)
    set(PLATFORM_DIRS
            src/psp2
            src/psp2/libvita2d/include
            src/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            src/psp2/*.c*
            src/psp2/libvita2d/source/*.c
            source/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            src/psp2/libvita2d/shader/compiled/*.o
            src/psp2/vita-shader-collection/lib/*.o
            )
    list(APPEND FLAGS -Wl,-q -O3 -D__PSP2__)
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
            src/ps3
            src/ps3/tiny3d/source
            src/ps3/tiny3d/include
            ${PSL1GHT}/portlibs/ppu/include/freetype2
            src/posix
            )
    file(GLOB PLATFORM_SRC
            src/ps3/*.c*
            src/ps3/tiny3d/source/*.c*
            src/ps3/tiny3d/source/*.S*
            src/posix/*.c*
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
            src/3ds
            src/posix
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SRC
            src/3ds/*.c*
            src/posix/*.c*
            source/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/3ds/vshaders.v.o
            )
    list(APPEND FLAGS -O3 -DARM11 -D_3DS -D__3DS__
            -D__CITRO3D__
            -ffunction-sections -fomit-frame-pointer
            )
    #####################
    # NX PLATORM
    #####################
elseif (BUILD_NX)
    set(PLATFORM_DIRS
            src/nx
            ${DEVKITPRO}/libnx/include
            )
    file(GLOB PLATFORM_SRC
            src/nx/*.c*
            )
    list(APPEND FLAGS -D_NX -D__NX__
            -ffunction-sections -fomit-frame-pointer
            )
    #####################
    # SDL2 PLATORM
    #####################
elseif (BUILD_SDL2)
    set(PLATFORM_DIRS
            source/sdl2
            source/posix
            )
    file(GLOB PLATFORM_SRC
            #src/sdl2/*.c*
            source/sdl2/sdl2_renderer.cpp
            source/posix/*.c*
            )
    list(APPEND FLAGS -O3 -D__SDL2__)
    #####################
    # SDL1 PLATORM
    #####################
elseif (BUILD_SDL1)
    set(PLATFORM_DIRS
            src/sdl1
            src/posix
            )
    file(GLOB PLATFORM_SRC
            src/sdl1/*.c*
            src/posix/*.c*
            )
    list(APPEND FLAGS -O3 -D__SDL1__)
    #####################
    # SFML PLATORM
    #####################
elseif (BUILD_SFML)
    set(PLATFORM_DIRS
            src/sfml
            src/posix
            )
    file(GLOB PLATFORM_SRC
            src/sfml/*.c*
            src/posix/*.c*
            source/sdl2/sdl2_audio.cpp #TODO: get rid of sdl2, use sdl2 audio for now
            )
    list(APPEND FLAGS -O3 -D__SFML__)
endif (BUILD_PSP2)


add_library(${PROJECT_NAME} ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CROSS2D_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PLATFORM_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

#####################
# SDL2 (unix) target
#####################
add_executable(${PROJECT_NAME}.sdl2 test/main.cpp)
target_compile_options(${PROJECT_NAME}.sdl2 PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.sdl2
        cross2d SDL2 SDL2_image SDL2_ttf z)
add_dependencies(${PROJECT_NAME}.sdl2 ${PROJECT_NAME})

#####################
# SDL1 (unix) target
#####################
add_executable(${PROJECT_NAME}.sdl1 test/main.cpp)
target_compile_options(${PROJECT_NAME}.sdl1 PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.sdl1
        cross2d SDL SDL_image SDL_gfx SDL_ttf z)
add_dependencies(${PROJECT_NAME}.sdl1 ${PROJECT_NAME})

#####################
# PS3 target
#####################
add_executable(${PROJECT_NAME}.ps3 test/main.cpp)
target_compile_options(${PROJECT_NAME}.ps3 PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME}.ps3 PUBLIC ${CROSS2D_DIR})
target_include_directories(${PROJECT_NAME}.ps3 PRIVATE ${PLATFORM_DIRS})
target_link_libraries(${PROJECT_NAME}.ps3 cross2d ${PLATFORM_LIBS})
add_dependencies(${PROJECT_NAME}.ps3 ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}.ps3.elf
        DEPENDS ${PROJECT_NAME}.ps3
        COMMAND ${PSL1GHT}/bin/sprxlinker ${PROJECT_NAME}.ps3
        )

#####################
# SFML (unix) target
#####################
add_executable(${PROJECT_NAME}.sfml test/main.cpp)
target_compile_options(${PROJECT_NAME}.sfml PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.sfml
        cross2d SDL2 sfml-graphics sfml-window sfml-system GL png z)
add_dependencies(${PROJECT_NAME}.sfml ${PROJECT_NAME})

#####################
# PSP2 (vita) target
#####################
add_executable(${PROJECT_NAME}.psp2 test/main.cpp)
target_compile_options(${PROJECT_NAME}.psp2 PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.psp2
        cross2d SDL2
        SceDisplay_stub SceGxm_stub SceSysmodule_stub
        SceCtrl_stub ScePgf_stub SceNetCtl_stub
        SceNet_stub ScePower_stub SceCommonDialog_stub
        SceAudio_stub SceAppMgr_stub SceTouch_stub
        stdc++ png jpeg z m c
        )
add_dependencies(${PROJECT_NAME}.psp2 ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}.psp2.vpk
        DEPENDS ${PROJECT_NAME}.psp2
        COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
        COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.psp2 ${PROJECT_NAME}.velf
        COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf vpk/eboot.bin
        COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLE_ID} "${PROJECT_NAME}" vpk/sce_sys/param.sfo
        COMMAND cp -r res/* vpk/
        COMMAND cd vpk && zip -r ../${PROJECT_NAME}-${BUILD_DATE}.vpk . && cd ..
        )

#####################
# CTR (3DS) target
#####################
if (BUILD_3DS)
    add_executable(${PROJECT_NAME}.3ds test/main.cpp)
    target_compile_options(${PROJECT_NAME}.3ds PRIVATE ${FLAGS})
    target_include_directories(${PROJECT_NAME}.3ds PUBLIC ${CROSS2D_DIR})
    target_include_directories(${PROJECT_NAME}.3ds PRIVATE ${PLATFORM_DIRS})
    target_link_libraries(${PROJECT_NAME}.3ds
            cross2d
            ${DEVKITPRO}/portlibs/3ds/lib/libSDL2.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libz.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
    set_target_properties(${PROJECT_NAME}.3ds PROPERTIES LINK_FLAGS "-specs=3dsx.specs")
    add_dependencies(${PROJECT_NAME}.3ds ${PROJECT_NAME})
    add_3dsx_target(${PROJECT_NAME}.3ds NO_SMDH)
endif (BUILD_3DS)

#####################
# NX (SWITCH) target
#####################
add_executable(${PROJECT_NAME}.nx test/main.cpp)
target_compile_options(${PROJECT_NAME}.nx PRIVATE ${FLAGS} -I${DEVKITPRO}/libnx/include)
target_link_libraries(${PROJECT_NAME}.nx
        cross2d
        ${DEVKITPRO}/libnx/lib/libnx.a
        m)
set_target_properties(${PROJECT_NAME}.nx PROPERTIES LINK_FLAGS "-specs=switch.specs")
add_dependencies(${PROJECT_NAME}.nx ${PROJECT_NAME})

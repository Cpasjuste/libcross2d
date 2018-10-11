cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support (hardware rendering, fast)" OFF)
option(BUILD_SDL1 "Build with SDL1 support (software, osmesa) support" OFF)
option(BUILD_SDL1_GL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)" OFF)
option(BUILD_SDL2 "Build with SDL2 support" OFF)
option(BUILD_SDL2_SOFT "Build with SDL2 support (software rendering, needs BUILD_SDL2=ON)" OFF)
option(BUILD_VITA "Build with Ps Vita support (hardware rendering, fast)" OFF)
option(BUILD_PS3 "Build with PS3 support (hardware rendering, fast)" OFF)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_SWITCH "Build with Nintendo Switch support" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()

# setup toolchain
set(CMAKE_TOOLCHAIN_FILE Toolchain.cmake)

project(cross2d)

# flags
set(FLAGS -Wall)

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
            SDL2
            sfml-graphics sfml-window sfml-system
            GL freetype z)
    #####################
    # VITA PLATORM
    #####################
elseif (BUILD_VITA)
    set(TITLE_ID CROSS0001)
    set(PLATFORM_DIRS
            source/platforms/posix
            source/platforms/psp2
            source/platforms/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
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
    #####################
    # PS3 PLATORM
    #####################
elseif (BUILD_PS3)
    set(CMAKE_C_FLAGS_RELEASE "-O2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    set(PLATFORM_DIRS
            source/platforms/ps3
            source/platforms/ps3/tiny3d
            include/cross2d/platforms/ps3/tiny3d
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
            #${PSL1GHT}/portlibs/ppu/lib/libpng.a
            ${PSL1GHT}/portlibs/ppu/lib/libtiff.a
            ${PSL1GHT}/portlibs/ppu/lib/libjpeg.a
            ${PSL1GHT}/ppu/lib/libgcm_sys.a
            ${PSL1GHT}/ppu/lib/librsx.a
            ${PSL1GHT}/ppu/lib/libsysutil.a
            ${PSL1GHT}/ppu/lib/libsysmodule.a
            #${PSL1GHT}/ppu/lib/libpngdec.a
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
            ${DEVKITPRO}/portlibs/3ds/include/freetype2
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    file(GLOB PLATFORM_SRC
            source/platforms/3ds/*.c*
            source/platforms/posix/*.c*
            #source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            source/platforms/3ds/vshaders.v.o
            source/platforms/3ds/render2d.shbin.o
            )

    set(PLATFORM_LIBS
            #${DEVKITPRO}/portlibs/3ds/lib/libSDL2.a
            ${DEVKITPRO}/portlibs/3ds/lib/libfreetype.a
            ${DEVKITPRO}/portlibs/3ds/lib/libpng.a
            ${DEVKITPRO}/portlibs/3ds/lib/libbz2.a
            ${DEVKITPRO}/portlibs/3ds/lib/libz.a
            #${DEVKITPRO}/libctru/lib/libcitro2d.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
    list(APPEND FLAGS -O3 -DARM11 -D_3DS -D__3DS__ -D__CITRO3D__
            -ffunction-sections -fomit-frame-pointer
            )
    #####################
    # SWITCH PLATORM
    #####################
elseif (BUILD_SWITCH)
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
            )
    set(PLATFORM_LIBS
            SDL2 glad EGL glapi drm_nouveau nx
            freetype bz2 png z
            )
    #####################
    # SDL2 PLATORM
    #####################
elseif (BUILD_SDL2)
    find_package(ZLIB REQUIRED)
    find_package(Freetype REQUIRED)
    set(PLATFORM_DIRS
            source/platforms/sdl2
            source/platforms/posix
            ${FREETYPE_INCLUDE_DIRS}
            ${ZLIB_INCLUDE_DIRS})
    file(GLOB PLATFORM_SRC
            source/platforms/sdl2/*.c*
            source/platforms/posix/*.c*)
    set(PLATFORM_LIBS SDL2 ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES})
    list(APPEND FLAGS -D__SDL2__)
    if (BUILD_SDL2_SOFT)
    else ()
        list(APPEND PLATFORM_DIRS source/platforms/gl)
        file(GLOB SHADERS_SRC source/platforms/gl/shaders/*.c*)
        list(APPEND PLATFORM_SRC
                source/platforms/gl/gl_renderer.cpp
                source/platforms/gl/gl_texture.cpp
                source/platforms/gl/gl_shaders.cpp
                ${SHADERS_SRC}
                )
        list(APPEND FLAGS -D__SDL2_GL__ -D__GL__)
        list(APPEND PLATFORM_LIBS GL)
    endif (BUILD_SDL2_SOFT)
    #####################
    # SDL1 PLATORM
    #####################
elseif (BUILD_SDL1)
    find_package(ZLIB REQUIRED)
    #find_package(PNG REQUIRED)
    find_package(Freetype REQUIRED)
    set(PLATFORM_DIRS
            source/platforms/sdl1
            source/platforms/gl
            source/platforms/posix
            ${FREETYPE_INCLUDE_DIRS}
            #${PNG_INCLUDE_DIRS}
            ${ZLIB_INCLUDE_DIRS})
    file(GLOB PLATFORM_SRC
            source/platforms/sdl1/*.c*
            source/platforms/gl/*.c*
            source/platforms/posix/*.c*)
    set(PLATFORM_LIBS SDL ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES})
    #${PNG_LIBRARIES}
    list(APPEND FLAGS -D__SDL1__ -D__GL__)
    if (BUILD_SDL1_GL)
        list(APPEND FLAGS -D__SDL1_GL__)
        list(APPEND PLATFORM_LIBS GL)
    else ()
        list(APPEND FLAGS -D__MESAGL__)
        list(APPEND PLATFORM_LIBS OSMesa pthread ${ZLIB_LIBRARIES})
    endif (BUILD_SDL1_GL)
endif (BUILD_SFML)

add_library(${PROJECT_NAME} ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CROSS2D_DIR} ${PLATFORM_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PLATFORM_LIBS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FLAGS} -Wno-narrowing)

#install(FILES ${CMAKE_BINARY_DIR}/libcross2d.a DESTINATION lib)
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/cross2d DESTINATION include)

#####################
# test executable
#####################
add_executable(${PROJECT_NAME}.elf test/main.cpp)
target_link_libraries(${PROJECT_NAME}.elf cross2d)

#############
# targets
#############
include(Targets.cmake)

cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
option(PLATFORM_LINUX "Linux platform target" OFF)
option(PLATFORM_WINDOWS "Windows platform target (msys/mingw64)" OFF)
option(PLATFORM_SWITCH "Nintendo Switch platform target" OFF)
option(PLATFORM_3DS "Nintendo 3DS platform target" OFF)
option(PLATFORM_VITA "Sony PS Vita platform target" OFF)
option(PLATFORM_DREAMCAST "Sega Dreamcast platform target" OFF)

####################
# BUILD OPTIONS
####################
option(OPTION_SDL1 "SDL1 support" OFF)
option(OPTION_SDL2 "SDL2 support" OFF)
option(OPTION_RENDER_GL1 "OpenGL 1.2 rendering" OFF)
option(OPTION_RENDER_GL2 "OpenGL 3.3 rendering" OFF)
option(OPTION_LOADER_GLAD "Glad OpenGL loader" ON)
option(OPTION_LOADER_GLEW "Glew OpenGL loader" OFF)
option(OPTION_RENDER_GLES2 "OpenGLES 2.0 rendering" OFF)
option(OPTION_RENDER_GLES2 "OpenGLES 2.0 rendering" OFF)
option(OPTION_BUILTIN_LIBCONFIG "Build libconfig from sources" OFF)
set(OPTION_SCREEN_WIDTH 1280 CACHE STRING "Custom screen width")
set(OPTION_SCREEN_HEIGHT 720 CACHE STRING "Custom screen height")

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

if (PLATFORM_DREAMCAST)
    message(STATUS "C2D: DREAMCAST OpenGL 1.2 support enabled")
    set(OPTION_RENDER_GL1 ON CACHE BOOL "OpenGL 1.2 rendering" FORCE)
    set(OPTION_SCREEN_WIDTH 640 CACHE STRING "Custom screen width" FORCE)
    set(OPTION_SCREEN_HEIGHT 480 CACHE STRING "Custom screen height" FORCE)
endif ()

####################
# COMMON STUFF
####################

# setup target toolchain
include(cmake/toolchain.cmake)

project(cross2d)

file(GLOB C2D_SOURCES
        source/widgets/*.c*
        source/skeleton/*.c*
        source/skeleton/sfml/*.c*)

# deps
if (NOT PLATFORM_DREAMCAST)
    include(FindPkgConfig)
    pkg_search_module(FREETYPE REQUIRED freetype2)
    pkg_search_module(PNG REQUIRED libpng)
    pkg_search_module(ZLIB REQUIRED zlib)
endif ()

set(C2D_CFLAGS -Wall -Wno-narrowing -DLIBCONFIG_STATIC)
set(C2D_INCLUDES include ${FREETYPE_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(C2D_LDFLAGS ${FREETYPE_LIBRARIES} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES})
set(CMAKE_CXX_STANDARD 17)

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

# gl2 / gles2
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

# gl1
if (OPTION_RENDER_GL1)
    file(GLOB GL_SRC source/platforms/gl1/*.c*)
    list(APPEND C2D_SOURCES ${GL_SRC})
    list(APPEND C2D_CFLAGS -D__GL1__)
endif ()

# glad/glew
if (OPTION_RENDER_GL1 OR OPTION_RENDER_GL2 OR OPTION_RENDER_GLES2)
    if (OPTION_LOADER_GLEW)
        set(GLEW_USE_STATIC_LIBS ON)
        find_package(GLEW REQUIRED)
        list(APPEND C2D_CFLAGS -D__GLEW__ -DGLEW_STATIC)
        list(APPEND C2D_LDFLAGS glew32)
    elseif (OPTION_LOADER_GLAD)
        set(GLAD_NO_LOADER ON CACHE BOOL "No loader")
        add_subdirectory(source/platforms/glad)
        list(APPEND PLATFORM_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/source/platforms/glad/include)
        list(APPEND C2D_CFLAGS -D__GLAD__)
        list(APPEND C2D_LDFLAGS glad)
    endif ()
endif ()

# export tools
set(ZIP zip CACHE STRING "zip executable path")

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
            )
    file(GLOB PLATFORM_SOURCES
            source/platforms/posix/posix_io.cpp
            source/platforms/psp2/*.c*
            source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            source/platforms/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            source/platforms/sdl2/sdl2_thread.cpp
            source/platforms/sdl2/sdl2_mutex.cpp
            source/platforms/psp2/vita-shader-collection/lib/*.o
            )
    list(APPEND C2D_LDFLAGS
            SDL2 vita2d jpeg png
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceAppMgr_stub SceCtrl_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub SceHid_stub SceMotion_stub
            z c stdc++ pthread m
            )
    list(APPEND C2D_CFLAGS
            -D__PSP2__ -DNO_KEYBOARD
            -ffast-math -mtune=cortex-a9 -mfpu=neon -flto -ftree-vectorize
            )
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
            )
    file(GLOB PLATFORM_SOURCES
            source/platforms/3ds/*.c*
            source/platforms/posix/*.c*
            #source/platforms/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            # TODO: fix that crap...
            #source/platforms/3ds/vshaders.v.o
            #source/platforms/3ds/render2d.shbin.o
            )
    list(APPEND C2D_LDFLAGS citro2d citro3d ctru m)
    list(APPEND C2D_CFLAGS -DARM11 -D_3DS -D__3DS__ -D__CITRO3D__ -Wno-pedantic)
    #set(CMAKE_CXX_STANDARD 20 CACHE STRING "CMAKE_CXX_STANDARD" FORCE)
    #####################
    # SWITCH PLATORM
    #####################
elseif (PLATFORM_SWITCH)
    set(PLATFORM_INCLUDES
            source/platforms/posix
            source/platforms/switch)
    file(GLOB PLATFORM_SOURCES
            source/platforms/posix/posix_io.cpp
            source/platforms/posix/posix_clock.cpp
            source/platforms/switch/*.c*)
    list(APPEND C2D_CFLAGS -D__SWITCH__ -DNO_KEYBOARD -Wno-int-in-bool-context)
    #set(CMAKE_CXX_STANDARD 20 CACHE STRING "CMAKE_CXX_STANDARD" FORCE)
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
    list(APPEND C2D_LDFLAGS c kallisti kosutils kosext2fs GLdc c freetype z m) # kosfat
    ########################
    # LINUX PLATORM
    ########################
elseif (PLATFORM_LINUX)
    file(GLOB PLATFORM_SOURCES source/platforms/posix/*.c*)
    # romfs: physfs
    pkg_search_module(PHYSFS REQUIRED physfs)
    # romfs: funchook
    add_subdirectory(source/skeleton/romfs/funchook)
    # romfs: physfs wrapper
    list(APPEND PLATFORM_SOURCES source/skeleton/romfs/romfs.cpp)
    list(APPEND C2D_INCLUDES ${PHYSFS_INCLUDE_DIRS})
    list(APPEND C2D_CFLAGS -D__PHYSFS_HOOKS__)
    list(APPEND C2D_LDFLAGS ${PHYSFS_LIBRARIES} funchook-static)
    set(PHYSFS_HOOKS ON CACHE BOOL "" FORCE)
    if (PLATFORM_WINDOWS)
        # hum... static linking, not easy on mingw64 :)
        list(APPEND C2D_LDFLAGS freetype brotlidec-static brotlicommon-static
                harfbuzz graphite2 rpcrt4 dwrite bz2 png z
                ole32 oleaut32 imm32 winmm version setupapi gdi32 opengl32)
        # TODO: fix zip path...
        set(ZIP "C:\\msys64\\usr\\bin\\zip.exe" CACHE STRING "zip executable path" FORCE)
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

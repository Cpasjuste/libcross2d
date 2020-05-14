# setup toolchains

if (PLATFORM_LINUX)
    set(CMAKE_SYSTEM_NAME "Linux")
    set(TARGET_PLATFORM linux CACHE STRING "")
endif ()
if (PLATFORM_RPI3)
    set(PLATFORM_LINUX ON CACHE BOOL "" FORCE)
    set(TARGET_PLATFORM rpi3 CACHE STRING "" FORCE)
endif ()
if (PLATFORM_DREAMCAST)
    set(TARGET_PLATFORM dreamcast CACHE STRING "" FORCE)

    set(CMAKE_SYSTEM_NAME "Generic")
    set(CMAKE_SYSTEM_PROCESSOR "sh")

    set(SH_ELF_PREFIX /opt/toolchains/dc/sh-elf)
    set(KOS_BASE $ENV{KOS_BASE} CACHE STRING "" FORCE)
    set(KOS_ARCH "dreamcast" CACHE STRING "" FORCE)
    set(KOS_SUBARCH "pristine" CACHE STRING "" FORCE)
    set(KOS_AS "${SH_ELF_PREFIX}/bin/sh-elf-as" CACHE STRING "" FORCE)
    set(KOS_AFLAGS "-little" CACHE STRING "" FORCE)
    set(KOS_LD "${SH_ELF_PREFIX}/bin/sh-elf-ld" CACHE STRING "" FORCE)
    set(KOS_OBJCOPY "${SH_ELF_PREFIX}/bin/sh-elf-objcopy" CACHE STRING "" FORCE)

    set(CMAKE_OBJCOPY ${SH_ELF_PREFIX}/bin/sh-elf-objcopy CACHE FILEPATH "")
    set(CMAKE_C_COMPILER ${SH_ELF_PREFIX}/bin/sh-elf-gcc CACHE FILEPATH "")
    set(CMAKE_CXX_COMPILER ${SH_ELF_PREFIX}/bin/sh-elf-g++ CACHE FILEPATH "")
    set(CMAKE_AR ${SH_ELF_PREFIX}/bin/sh-elf-ar CACHE FILEPATH "")
    set(CMAKE_ASM_COMPILER ${SH_ELF_PREFIX}/bin/sh-elf-gcc CACHE FILEPATH "")
    set(CMAKE_LINKER ${SH_ELF_PREFIX}/bin/sh-elf-ld CACHE FILEPATH "")

    set(KOS_INC_PATHS "-I${KOS_BASE}/include -I${KOS_BASE}/kernel/arch/dreamcast/include -I${KOS_BASE}/addons/include -I${KOS_BASE}/../kos-ports/include")
    set(KOS_LIB_PATHS "-L${KOS_BASE}/lib/${KOS_ARCH} -L${KOS_BASE}/addons/lib/${KOS_ARCH} -L${KOS_BASE}/../kos-ports/lib")
    set(KOS_LIBS "-Wl,--start-group -lc -lgcc -Wl,--end-group")
    set(KOS_CFLAGS "-D__DREAMCAST__ -O2 -ml -m4-single-only -ffunction-sections -fdata-sections -D_arch_dreamcast -D_arch_sub_pristine -Wall -g -fno-builtin")

    set(CMAKE_C_FLAGS "${KOS_CFLAGS} ${KOS_INC_PATHS}" CACHE STRING "C flags" FORCE)
    set(CMAKE_CXX_FLAGS "${KOS_CFLAGS} ${KOS_INC_PATHS} -std=gnu++11 -fno-operator-names -fno-rtti -fno-exceptions" CACHE STRING "C++ flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "CMAKE_C_FLAGS_RELEASE" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "CMAKE_CXX_FLAGS_RELEASE" FORCE)

    set(CMAKE_C_LINK_FLAGS "${KOS_LIB_PATHS} ${KOS_LIBS} -T${KOS_BASE}/utils/ldscripts/shlelf.xc ${KOS_LIBS}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_LINK_FLAGS "${KOS_LIB_PATHS} ${KOS_LIBS} -T${KOS_BASE}/utils/ldscripts/shlelf.xc ${KOS_LIBS}" CACHE STRING "" FORCE)

    set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <OBJECTS> ${CMAKE_C_FLAGS} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES> -o <TARGET>")
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} <OBJECTS> ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_LINK_FLAGS} <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES> -o <TARGET>")

    set_property(DIRECTORY PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
    set(CMAKE_FIND_ROOT_PATH ${SH_ELF_PREFIX} $ENV{KOS_BASE}/ $ENV{KOS_BASE}/addons $ENV{KOS_BASE}/../kos-ports)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_EXECUTABLE_SUFFIX_C ".elf" CACHE STRING "" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf" CACHE STRING "" FORCE)
endif ()
if (PLATFORM_WINDOWS)
    set(PLATFORM_LINUX ON CACHE BOOL "" FORCE)
    set(PLATFORM_WINDOWS ON CACHE BOOL "" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__WINDOWS__" CACHE STRING "C flags" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WINDOWS__" CACHE STRING "CXX flags" FORCE)
    set(TARGET_PLATFORM windows CACHE STRING "" FORCE)
elseif (PLATFORM_SWITCH)
    set(DEVKITPRO $ENV{DEVKITPRO} CACHE BOOL "DEVKITPRO")
    set(CMAKE_SYSTEM_NAME "Generic")
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(CMAKE_EXE_LINKER_FLAGS_INIT "-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -ftls-model=local-exec -L/opt/devkitpro/portlibs/switch/lib -L/opt/devkitpro/libnx/lib -specs=/opt/devkitpro/libnx/switch.specs")
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_INIT} CACHE STRING "" FORCE)
    set(CMAKE_C_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-g++")
    set(CMAKE_ASM_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-as")
    set(CMAKE_AR "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ar" CACHE FILEPATH "")
    set(CMAKE_RANLIB "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ranlib")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${DEVKITPRO}/libnx/include -I${DEVKITPRO}/portlibs/switch/include" CACHE STRING "C flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fpermissive" CACHE STRING "C++ flags")
    set(ENV{PKG_CONFIG_DIR} "")
    set(ENV{PKG_CONFIG_LIBDIR} "${DEVKITPRO}/portlibs/switch/lib/pkgconfig")
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${DEVKITPRO}/portlibs/switch")
    set(CMAKE_FIND_ROOT_PATH ${DEVKITPRO}/libnx ${DEVKITPRO}/portlibs/switch)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    set(TARGET_PLATFORM switch CACHE STRING "")
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not wanted")
elseif (PLATFORM_VITA)
    if (NOT DEFINED ENV{VITASDK})
        set(ENV{VITASDK} "/usr/local/vitasdk")
    endif ()
    include("$ENV{VITASDK}/share/vita.toolchain.cmake")
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    set(TARGET_PLATFORM vita CACHE STRING "" FORCE)
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "C++ Release flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "C Release flags" FORCE)
    set(CMAKE_AR "${VITASDK}/bin/arm-vita-eabi-gcc-ar" CACHE PATH "archive" FORCE)
    set(CMAKE_RANLIB "${VITASDK}/bin/arm-vita-eabi-gcc-ranlib${TOOL_OS_SUFFIX}" CACHE PATH "ranlib" FORCE)
elseif (PLATFORM_3DS)
    set(DEVKITPRO $ENV{DEVKITPRO} CACHE BOOL "DEVKITPRO")
    include(${DEVKITPRO}/3ds.cmake)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_INIT} CACHE STRING "" FORCE)
    set(CMAKE_C_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-g++")
    set(CMAKE_ASM_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-as")
    set(CMAKE_AR "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-ar")
    set(CMAKE_RANLIB "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-gcc-ranlib")
    set(CMAKE_C_FLAGS "-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mword-relocations -ffunction-sections -fdata-sections -I${DEVKITPRO}/libctru/include -I${DEVKITPRO}/portlibs/3ds/include" CACHE STRING "C flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fpermissive" CACHE STRING "C++ flags")
    set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -L${DEVKITPRO}/portlibs/3ds/lib -L${DEVKITPRO}/libctru/lib")
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_INIT} CACHE STRING "" FORCE)
    set(ENV{PKG_CONFIG_DIR} "")
    set(ENV{PKG_CONFIG_LIBDIR} "${DEVKITPRO}/portlibs/3ds/lib/pkgconfig")
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${DEVKITPRO}/portlibs/3ds")
    set(CMAKE_FIND_ROOT_PATH ${DEVKITPRO}/libctru ${DEVKITPRO}/portlibs/3ds)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    set(TARGET_PLATFORM 3ds CACHE STRING "")
endif ()

message(STATUS "C2D: target platform: ${TARGET_PLATFORM}, cmake version: ${CMAKE_VERSION}")

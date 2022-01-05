cmake_minimum_required(VERSION 3.1)
#set(CMAKE_VERBOSE_MAKEFILE ON)

###########################################################
# Copy data to binary directory (common to all platforms)
###########################################################
add_custom_target(${PROJECT_NAME}.data
        # cleanup data in binary dir
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir
        # copy data to binary directory, for program execution when invoked from cmake build directory
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        # cache data to binary directory
        # this allow parent projects to add files here before packaging (make project-name_target_release)
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data)

########################
# Linux/Win64 targets
########################
if (PLATFORM_LINUX OR PLATFORM_WINDOWS)
    # romfs
    include(${cross2d_SOURCE_DIR}/cmake/romfs.cmake)
    add_romfs(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/data_romfs)
    add_dependencies(${PROJECT_NAME}-romfs ${PROJECT_NAME}.data)
    # release
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif ()

###########################
# Nintendo Switch target
###########################
if (PLATFORM_SWITCH)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${DEVKITPRO}/tools/bin/nacptool --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}" ${PROJECT_NAME}.nacp
            # copy custom switch "romfs" data to common "romfs" data for romfs creation (merge/overwrite common data)
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
            COMMAND ${DEVKITPRO}/tools/bin/elf2nro ${PROJECT_NAME} ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/icon.jpg --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_CURRENT_BINARY_DIR}/data_romfs)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif (PLATFORM_SWITCH)

#####################
# VITA target
#####################
if (PLATFORM_VITA)
    add_custom_target(${PROJECT_NAME}.vpk
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            # create eboot
            COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.velf
            COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf eboot.bin
            # create vpk
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/vpk
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/vpk
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/vpk -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND ${CMAKE_COMMAND} -E copy eboot.bin ${CMAKE_CURRENT_BINARY_DIR}/vpk/eboot.bin
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="${TITLE_ID}" "${PROJECT_NAME}" ${CMAKE_CURRENT_BINARY_DIR}/vpk/sce_sys/param.sfo
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/vpk && ${ZIP} -r ../${PROJECT_NAME}.vpk .
            )
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.vpk
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vpk ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif (PLATFORM_VITA)

###########################
# PS4 target
###########################
if (PLATFORM_PS4)
    #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG -s)
    # SELF
    #add_self(${PROJECT_NAME})

    set(AUTH_INFO "000000000000000000000000001C004000FF000000000080000000000000000000000000000000000000008000400040000000000000008000000000000000080040FFFF000000F000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000")
    add_custom_command(
            OUTPUT "${PROJECT_NAME}.self"
            COMMAND ${CMAKE_COMMAND} -E env "ORBISDEV=${ORBISDEV}" "${ORBISDEV}/bin/orbis-elf-create" "${PROJECT_NAME}" "${PROJECT_NAME}.oelf"
            COMMAND "python" "${ORBISDEV}/bin/make_fself.py" "--auth-info" "${AUTH_INFO}" "${PROJECT_NAME}.oelf" "${PROJECT_NAME}.self"
            VERBATIM
            DEPENDS "${PROJECT_NAME}"
    )
    add_custom_target(
            "${PROJECT_NAME}_self" ALL
            DEPENDS "${PROJECT_NAME}.self"
    )

    add_dependencies(${PROJECT_NAME}_self ${PROJECT_NAME}.data)
    # ROMFS FILE LIST (for gp4gen) (TODO: properly do this...)
    execute_process(
            COMMAND bash -c "[ -d ${CMAKE_CURRENT_BINARY_DIR}/data_romfs ] && cd ${CMAKE_CURRENT_BINARY_DIR}/data_romfs/ && find . -type f | cut -d '/' -f2- | tr '\n' ','"
            OUTPUT_VARIABLE C2D_ROMFS_FILES
    )
    # PKG
    add_custom_command(
            OUTPUT "${PROJECT_NAME}.pkg"
            # copy self to romfs dir (eboot)
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.self" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/eboot.bin"
            # update sfo
            COMMAND "${ORBISDEV}/bin/orbis-sfo" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/sce_sys/param.sfo" "-w" "content_id" "IV0001-${PS4_PKG_TITLE_ID}_00-${PS4_PKG_TITLE_ID}0000000"
            COMMAND "${ORBISDEV}/bin/orbis-sfo" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/sce_sys/param.sfo" "-w" "title_id" "${PS4_PKG_TITLE_ID}"
            COMMAND "${ORBISDEV}/bin/orbis-sfo" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/sce_sys/param.sfo" "-w" "title" "${PS4_PKG_TITLE}"
            COMMAND "${ORBISDEV}/bin/orbis-sfo" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/sce_sys/param.sfo" "-w" "version" "${PS4_PKG_VERSION}"
            COMMAND "${ORBISDEV}/bin/orbis-sfo" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/sce_sys/param.sfo" "-w" "app_ver" "${PS4_PKG_VERSION}"
            # generate gp4 file
            COMMAND "${ORBISDEV}/bin/gp4gen" "--content-id" "IV0001-${PS4_PKG_TITLE_ID}_00-${PS4_PKG_TITLE_ID}0000000" "--files" "${C2D_ROMFS_FILES}"
            COMMAND ${CMAKE_COMMAND} -E rename "${CMAKE_CURRENT_BINARY_DIR}/homebrew.gp4" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/homebrew.gp4"
            COMMAND ${CMAKE_COMMAND} -E env "ORBISDEV=${ORBISDEV}" "${ORBISDEV}/bin/pkgTool" "pkg_build" "${CMAKE_CURRENT_BINARY_DIR}/data_romfs/homebrew.gp4" "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
            DEPENDS "${PROJECT_NAME}.self"
    )
    add_custom_target(
            "${PROJECT_NAME}_pkg" ALL
            DEPENDS "${PROJECT_NAME}.pkg"
    )
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.pkg
            # TODO
            )
endif (PLATFORM_PS4)

###########################
# Sony ps3 target
###########################
if (PLATFORM_PS3)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    add_custom_target(${PROJECT_NAME}.self
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME} ${PROJECT_NAME}.sprx
            COMMAND ${PSL1GHT}/bin/sprxlinker ${PROJECT_NAME}.sprx
            COMMAND ${PSL1GHT}/bin/make_self ${PROJECT_NAME}.sprx ${PROJECT_NAME}.self
            )
endif (PLATFORM_PS3)

###########################
# Dreamcast target
###########################
if (PLATFORM_DREAMCAST)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    # romdisk handling (tricky..)
    add_custom_target(
            ${PROJECT_NAME}.romdisk ALL
            DEPENDS dummy_romdisk
    )
    add_custom_command(OUTPUT
            dummy_romdisk ${CMAKE_BINARY_DIR}/romdisk.o
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
            COMMAND ${KOS_BASE}/utils/genromfs/genromfs -f ${CMAKE_BINARY_DIR}/romdisk.img -d ${CMAKE_CURRENT_BINARY_DIR}/data_romfs -v
            COMMAND KOS_ARCH=${KOS_ARCH} KOS_AS=${KOS_AS} KOS_AFLAGS=${KOS_AFLAGS} KOS_LD=${KOS_LD} KOS_OBJCOPY=${KOS_OBJCOPY}
            /bin/bash ${KOS_BASE}/utils/bin2o/bin2o ${CMAKE_BINARY_DIR}/romdisk.img romdisk ${CMAKE_BINARY_DIR}/romdisk.o
            )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/romdisk.o)
    add_custom_target(${PROJECT_NAME}.bin
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_OBJCOPY} -R .stack -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
            )
    add_custom_target(${PROJECT_NAME}.cdi
            DEPENDS ${PROJECT_NAME}.bin
            COMMAND scramble ${PROJECT_NAME}.bin 1DS_CORE.BIN
            COMMAND genisoimage -V ${PROJECT_NAME} -G ${CMAKE_SOURCE_DIR}/data/dreamcast/IP.BIN -joliet -rock -l -x .svn -o ${PROJECT_NAME}.iso 1DS_CORE.BIN ${CMAKE_CURRENT_BINARY_DIR}/data_datadir
            COMMAND cdi4dc ${PROJECT_NAME}.iso ${PROJECT_NAME}.cdi -d >> cdi4dc.log
            )
endif (PLATFORM_DREAMCAST)

#####################
# 3DS target
#####################
# TODO: update target and packaging, see linux/windows/switch
if (PLATFORM_3DS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-L${DEVKITPRO}/portlibs/3ds/lib -L${DEVKITPRO}/libctru/lib -specs=${DEVKITPRO}/devkitARM/arm-none-eabi/lib/3dsx.specs")
    add_custom_target(${PROJECT_NAME}.3dsx
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${DEVKITPRO}/tools/bin/smdhtool --create "${PROJECT_NAME}" "${PROJECT_NAME}" "${PROJECT_AUTHOR}" ${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/icon.png ${PROJECT_NAME}.smdh
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
            COMMAND ${DEVKITPRO}/tools/bin/3dsxtool ${PROJECT_NAME} ${PROJECT_NAME}.3dsx --smdh=${PROJECT_NAME}.smdh --romfs=${CMAKE_CURRENT_BINARY_DIR}/data_romfs)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.3dsx
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.3dsx ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif (PLATFORM_3DS)

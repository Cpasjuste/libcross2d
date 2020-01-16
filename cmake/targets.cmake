cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

###########################################################
# Copy data to binary directory (common to all platforms)
###########################################################
add_custom_target(${PROJECT_NAME}.data
        # cleanup data in binary dir
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir
        # copy data to binary directory, for program execution when invoked from cmake build directory
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
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
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            # on linux and windows, both data and romfs data are merged
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
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

###########################
# Nintendo Switch target
###########################
if (PLATFORM_DREAMCAST)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
    add_custom_target(${PROJECT_NAME}.bin
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_OBJCOPY} -R .stack -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
            #COMMAND ${DEVKITPRO}/tools/bin/nacptool --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}" ${PROJECT_NAME}.nacp
            # copy custom switch "romfs" data to common "romfs" data for romfs creation (merge/overwrite common data)
            #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_romfs
            #COMMAND ${DEVKITPRO}/tools/bin/elf2nro ${PROJECT_NAME} ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/data/${TARGET_PLATFORM}/icon.jpg --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_CURRENT_BINARY_DIR}/data_romfs)
            )
    add_custom_target(${PROJECT_NAME}.cdi
            DEPENDS ${PROJECT_NAME}.bin
            COMMAND scramble ${PROJECT_NAME}.bin 1DS_CORE.BIN
            COMMAND genisoimage -V ${PROJECT_NAME} -G ${CMAKE_CURRENT_SOURCE_DIR}/libcross2d/data/dreamcast/IP.BIN -joliet -rock -l -x .svn -o ${PROJECT_NAME}.iso 1DS_CORE.BIN
            COMMAND cdi4dc ${PROJECT_NAME}.iso ${PROJECT_NAME}.cdi -d >> cdi4dc.log
            )
endif (PLATFORM_DREAMCAST)

#####################
# 3DS target
#####################
# TODO: update target and packaging, see linux/windows/switch
if (PLATFORM_3DS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/devkitARM/arm-none-eabi/lib/3dsx.specs")
    add_custom_target(${PROJECT_NAME}.3dsx
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${DEVKITPRO}/tools/bin/smdhtool --create "${PROJECT_NAME}" "${PROJECT_NAME}" "${PROJECT_AUTHOR}" ../data/3ds/icon.png ${PROJECT_NAME}.smdh
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

#####################
# VITA target
#####################
if (PLATFORM_VITA)
    add_custom_target(${PROJECT_NAME}.vpk
            DEPENDS ${PROJECT_NAME}
            DEPENDS ${PROJECT_NAME}.data
            # create eboot
            COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.velf
            COMMAND ${VITASDK}/bin/vita-make-fself -s -c ${PROJECT_NAME}.velf eboot.bin
            # create vpk
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/vpk
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/vpk
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -D DST=${CMAKE_BINARY_DIR}/vpk -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND ${CMAKE_COMMAND} -E copy eboot.bin ${CMAKE_BINARY_DIR}/vpk/eboot.bin
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="${TITLE_ID}" "${PROJECT_NAME}" ${CMAKE_BINARY_DIR}/vpk/sce_sys/param.sfo
            COMMAND cd ${CMAKE_BINARY_DIR}/vpk && ${ZIP} -r ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.vpk ..
            )
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.vpk
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.vpk ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -D DST=${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} -P ${CMAKE_CURRENT_LIST_DIR}/copy_directory_custom.cmake
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif (PLATFORM_VITA)

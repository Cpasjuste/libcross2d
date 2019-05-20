cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

###########################################################
# Copy data to binary directory (common to all platforms)
###########################################################
add_custom_target(${PROJECT_NAME}.data
        # copy source data to binary directory for program execution from cmake build directory
        # on linux and windows, both ro (read only) and rw (read write) data are merged together
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/common/read_only ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/common/read_write ${CMAKE_CURRENT_BINARY_DIR}
        # copy source read only (ro) and read write data to binary directory,
        # this allow parent projects to add file here before packaging "make project-name_linux_release"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/common/read_only ${CMAKE_CURRENT_BINARY_DIR}/data_read_only
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/common/read_write ${CMAKE_CURRENT_BINARY_DIR}/data_read_write
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data)

########################
# Linux/Win64 targets
########################
if (BUILD_LINUX OR BUILD_WINDOWS)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            # on linux and windows, both ro (read only) and rw (read write) data are merged in the same folder (data)
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_only ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_${TARGET_PLATFORM}.zip ${PROJECT_NAME}
            )
endif ()

###########################
# Nintendo Switch target
###########################
if (BUILD_SWITCH)
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.data
            COMMAND ${DEVKITPRO}/tools/bin/nacptool --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}" ${PROJECT_NAME}.nacp
            # copy custom switch "read_only" data to common "read_only" data for romfs creation (merge/overwrite common data)
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/switch/read_only ${CMAKE_CURRENT_BINARY_DIR}/data_read_only
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/switch/read_write ${CMAKE_CURRENT_BINARY_DIR}/data_read_write
            COMMAND ${DEVKITPRO}/tools/bin/elf2nro ${PROJECT_NAME} ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/data/switch/icon.jpg --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_CURRENT_BINARY_DIR}/data_read_only)
    add_custom_target(${PROJECT_NAME}_${TARGET_PLATFORM}_release
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_switch.zip
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND cd ${CMAKE_BINARY_DIR}/release && ${ZIP} -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_switch.zip ${PROJECT_NAME}
            )
endif (BUILD_SWITCH)

#####################
# VITA target
#####################
# TODO: update target and packaging, see linux/windows/switch
if (BUILD_VITA)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
    add_custom_target(${PROJECT_NAME}_vita_release
            DEPENDS ${PROJECT_NAME}.self
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/data
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/sce_sys
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="${TITLE_ID}" "${PROJECT_NAME}" ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/sce_sys/param.sfo
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.self ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/eboot.bin
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/data/vita/. ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/data/read_only/. ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/data
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/data/read_write/. ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND cd ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME} && zip -r ../../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_vita.vpk .
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR})
endif (BUILD_VITA)

#####################
# 3DS target
#####################
# TODO: update target and packaging, see linux/windows/switch
if (BUILD_3DS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/devkitARM/arm-none-eabi/lib/3dsx.specs")
    add_custom_target(${PROJECT_NAME}.3dsx
            DEPENDS ${PROJECT_NAME}
            COMMAND ${DEVKITPRO}/tools/bin/smdhtool --create "${PROJECT_NAME}" "${PROJECT_NAME}" "${PROJECT_AUTHOR}" ${CMAKE_CURRENT_SOURCE_DIR}/data/3ds/icon.png ${PROJECT_NAME}.smdh
            COMMAND ${DEVKITPRO}/tools/bin/3dsxtool ${PROJECT_NAME} ${PROJECT_NAME}.3dsx --smdh=${PROJECT_NAME}.smdh --romfs=${CMAKE_CURRENT_SOURCE_DIR}/data/read_only)
    add_custom_target(${PROJECT_NAME}_3ds_release.zip
            DEPENDS ${PROJECT_NAME}.3dsx
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.3dsx ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/data/read_write/. ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_3ds.zip ${PROJECT_NAME}
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR})
endif (BUILD_3DS)

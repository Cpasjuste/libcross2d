cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

#####################
# PSP2 (vita) target
#####################
if (BUILD_PSP2)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
    add_custom_target(${PROJECT_NAME}_vita_release.zip
            DEPENDS ${PROJECT_NAME}.self
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/sce_sys
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="PNES0001" "pnes" ${CMAKE_BINARY_DIR}/release/pnes/sce_sys/param.sfo
            COMMAND cp -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.self ${CMAKE_BINARY_DIR}/release/pnes/eboot.bin
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/psp2/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cd ${CMAKE_BINARY_DIR}/release/pnes && zip -r ../../${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_vita.vpk . && cd ${CMAKE_BINARY_DIR}
            )
endif (BUILD_PSP2)

#####################
# NX (SWITCH) target
#####################
if (BUILD_NX)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND nacptool --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}" ${PROJECT_NAME}.nacp
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --nacp=${PROJECT_NAME}.nacp)
    # TODO: add default icon
    #--icon=${CMAKE_CURRENT_SOURCE_DIR}/data/nx/icon.jpg
    add_custom_target(${PROJECT_NAME}_switch_release.zip
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/${PROJECT_NAME}.nro
            #COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/data/skin ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            #COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/data/nx/skin ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}_switch.zip ${PROJECT_NAME}
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}
            )
endif (BUILD_NX)

 cmake_minimum_required(VERSION 3.2)

 # add romfs to project
 macro(add_romfs target romfs_dir)
     # find absolute romfs directory
     set(ROMFS_INPUT_DIR ${romfs_dir})
     if (NOT IS_ABSOLUTE ${romfs_dir})
         set(ROMFS_INPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${romfs_dir}")
     endif ()

     # build tar object file from directory
     add_custom_command(
             OUTPUT app.romfs.o
             COMMAND touch ${ROMFS_INPUT_DIR}/.keep
             COMMAND cd ${ROMFS_INPUT_DIR} && ${ZIP} -r ${CMAKE_CURRENT_BINARY_DIR}/romfs.zip .
             COMMAND ${CMAKE_LINKER} --relocatable --format binary --output app.romfs.o romfs.zip
             COMMAND ${CMAKE_COMMAND} -E remove -f romfs.zip
             DEPENDS ${ROMFS_INPUT_DIR}
     )

     # add static library
     add_library(${target}-romfs STATIC app.romfs.o)
     set_source_files_properties(app.romfs.o PROPERTIES EXTERNAL_OBJECT true GENERATED true)
     set_target_properties(${target}-romfs PROPERTIES LINKER_LANGUAGE C)
     target_link_libraries(${target} ${target}-romfs)
 endmacro()

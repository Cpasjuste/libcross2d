 cmake_minimum_required(VERSION 3.2)

 # add romfs to project
 macro(add_romfs target romfs_dir)
     # build tar object file from directory
     add_custom_command(
             OUTPUT ${target}.romfs.c
             COMMAND ${CMAKE_COMMAND} -E touch ${romfs_dir}/.keep
             COMMAND cd ${romfs_dir} && ${ZIP} -r ${CMAKE_CURRENT_BINARY_DIR}/${target}.romfs.zip .
             # https://github.com/gwilymk/bin2c
             COMMAND bin2c ${CMAKE_CURRENT_BINARY_DIR}/${target}.romfs.zip ${CMAKE_CURRENT_BINARY_DIR}/${target}.romfs.c c2d_romfs
             COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/${target}.romfs.zip
             DEPENDS ${romfs_dir}
     )

     # add static library
     add_library(${target}-romfs STATIC ${target}.romfs.c)
     # cross2d needs to be linked before target romfs
     target_link_libraries(${target} cross2d ${target}-romfs)
 endmacro()

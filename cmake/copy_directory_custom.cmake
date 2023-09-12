cmake_minimum_required(VERSION 3.5)

# copy a directory only if source exist
if (EXISTS ${SRC})
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_directory ${SRC} ${DST})
endif ()

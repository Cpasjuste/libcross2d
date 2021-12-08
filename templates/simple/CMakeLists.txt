cmake_minimum_required(VERSION 3.0)

########################
# LIBCROSS2D SUBMODULE
########################
add_subdirectory(libcross2d)

########################
# PROJECT SETUP
########################
project(PROJECT_NAME_TEMPLATE)
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(PROJECT_AUTHOR "$PROJECT_AUTHOR_TEMPLATE")
set(TITLE_ID "C2D000001") # ps vita

########################
# SOURCES SETUP
########################
file(GLOB SOURCES src/*.c*)
set(INCLUDES src)
set(CFLAGS)
set(LDFLAGS)

#####################
# EXECUTABLE
#####################
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${CFLAGS})
target_link_libraries(${CMAKE_PROJECT_NAME} cross2d ${LDFLAGS})

#####################
# targets
#####################
include(libcross2d/cmake/targets.cmake)
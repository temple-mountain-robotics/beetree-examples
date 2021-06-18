cmake_minimum_required(VERSION 3.14)
project(helloworld-workspace C CXX ASM)

# C/C++ Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Set the environment variable or add the command line argument -DBEETREE_DEV_DIR
# with your local Beetree repository directory to skip downloading from 
# the remote (github) and instead your local repository.
if(DEFINED ENV{BEETREE_DEV_DIR})
    message(STATUS "BEETREE_DEV_DIR environement variable detected")
    message(STATUS "Studio Bee's Beetree will be sourced from ${BEETREE_DEV_DIR}")
    set(BEETREE_DEV_DIR $ENV{BEETREE_DEV_DIR})
endif()

# Import Studio Bee's beetree
include(FetchContent)
set(FETCHCONTENT_SOURCE_DIR_STUDIOBEE_BEETREE ${BEETREE_DEV_DIR})
FetchContent_Declare(
    studiobee_beetree
    GIT_REPOSITORY https://github.com/Temple-Mountain-Robotics/beetree.git
)
FetchContent_MakeAvailable(studiobee_beetree)

# Add Beetree's module path and this project's module path!
list(APPEND CMAKE_MODULE_PATH ${studiobee_beetree_SOURCE_DIR}/application/cmake ${PROJECT_SOURCE_DIR}/cmake)

# Add the application
add_subdirectory(application)
add_subdirectory(platform)
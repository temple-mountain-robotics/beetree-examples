cmake_minimum_required(VERSION 3.14)
project(aries-workspace C CXX ASM)

# Policies
cmake_policy(SET CMP0076 NEW) # allow relative paths in target_sources
cmake_policy(SET CMP0077 NEW) # gtest pthreads behavior

# C/C++ Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Import BeeTree
message(STATUS "[helloworld] importing beetree from https://github.com/temple-mountain-robotics/beetree.git")
include(FetchContent)
FetchContent_Declare(
    beetree
    GIT_REPOSITORY https://github.com/temple-mountain-robotics/beetree.git
    GIT_TAG master
)
FetchContent_MakeAvailable(beetree)

# Add BeeTree's module path and this project's module path!
list(APPEND CMAKE_MODULE_PATH ${beetree_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/cmake)

# Add the application
add_subdirectory(application)
add_subdirectory(platform)
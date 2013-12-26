cmake_minimum_required (VERSION 2.6)

project(NaoFramework)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, defaulting to Debug. To change this call cmake with option -DCMAKE_BUILD_TYPE=[...]")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
# Flags to do in debug build
set(CMAKE_CXX_FLAGS_DEBUG "-DNAO_DEBUG -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NaoFramework_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${NaoFramework_BINARY_DIR})
# Output executable in main directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NaoFramework_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory (${PROJECT_SOURCE_DIR}/src)

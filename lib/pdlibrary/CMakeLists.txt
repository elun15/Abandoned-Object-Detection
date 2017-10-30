cmake_minimum_required(VERSION 2.8)

project(pdlibrary)
set(LIBNAME "pd")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# compilation mode setup
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(PD_out_dir ".")
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PD_out_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PD_out_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PD_out_dir})

# add some standard warnings
ADD_DEFINITIONS(-Wno-variadic-macros -Wno-long-long -Wall -Wextra -Winit-self -Woverloaded-virtual -Wsign-promo -Wno-unused-parameter -pedantic -Woverloaded-virtual -Wno-unknown-pragmas)

#check opencv
set(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  message(STATUS "OpenCV library status:")
  message(STATUS "    version: ${OpenCV_VERSION}")
  message(STATUS "    libraries: ${OpenCV_LIBS}")
  message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
endif()

if(${OpenCV_VERSION} VERSION_LESS 3.0.0)
  message(FATAL_ERROR "OpenCV version is not compatible: ${OpenCV_VERSION}")
endif()

# source files
file(GLOB_RECURSE DPM_SRC dpm/*.cpp)
file(GLOB_RECURSE ACF_SRC acf/*.cpp)

#include directories
file(GLOB_RECURSE PD_INCLUDE dpm/*.h dpm/*.hpp acf/*.h acf/*.hpp)

#add include directories
include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS} ${PD_include})

# Create a library called "libPD which includes the source files
# The extension is already found. Any number of sources could be listed here.
add_library(${LIBNAME} STATIC ${DPM_SRC} ${ACF_SRC})

# We add libraries from OpenCV...
target_link_libraries(${LIBNAME} LINK_PUBLIC ${OpenCV_LIBS})

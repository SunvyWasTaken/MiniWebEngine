cmake_minimum_required (VERSION 3.30.1)

set(CMAKE_DEBUG_POSTFIX "-d")

add_library(MiniMotor_compiler_flags INTERFACE)
target_compile_features(MiniMotor_compiler_flags INTERFACE cxx_std_20)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)

if(DEFINED ENV{EMSDK_HOME})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK_HOME}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" CACHE STRING "")
endif()

project(MiniMotorApp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

#option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(Game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
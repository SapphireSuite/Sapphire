# Copyright 2020 Sapphire development team. All Rights Reserved.

cmake_minimum_required(VERSION 3.12)



# === Projects ===

project(Sapphire)

message("Main directory: ${CMAKE_SOURCE_DIR}")






# === Outputs ===

# Setup output directories for project.
message("Output Directory: ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR}")

# .lib / .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR}/Libs")

# Module .dll / .so (add_library() with MODULE specifier).
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR}/Modules")

# .exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR}")



# === Tests ===

enable_testing()


# === Entrypoints ===

add_subdirectory(Engine)

add_subdirectory(Samples)

# Copyright 2020 Sapphire development team. All Rights Reserved.



# === Projects ===

project(Engine)



# === Inputs ===

# Parse files.
file(GLOB_RECURSE SOURCES "Source/*" "Include/*")

# Add dynamic library target.
add_library(Engine SHARED ${SOURCES})



# === Include directories ===

target_include_directories(Engine PUBLIC Include)
target_include_directories(Engine PUBLIC Include/Sapphire)



# === Input Libs ===

# Vulkan.
find_package(Vulkan REQUIRED FATAL_ERROR)
target_link_libraries(Engine Vulkan::Vulkan)


# GLFW.
target_link_libraries(Engine ${PROJECT_SOURCE_DIR}/Libs/glfw-3.3/lib-vc2019/glfw3.lib)
target_include_directories(Engine PRIVATE Libs/glfw-3.3/include)


# tiny header libs.
target_include_directories(Engine PRIVATE Libs/tiny)



# === Dependencies ===

# Enable Multithreading (-pthread).
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Engine Threads::Threads)



# === Compile features ===

# Clean default compilation flags.
if(MSVC)
	set(CMAKE_C_FLAGS "/DWIN32 /D_WINDOWS")
	set(CMAKE_C_FLAGS_RELEASE "/DNDEBUG")
	set(CMAKE_C_FLAGS_DEBUG "")

	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS")
	set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "")
endif()


# Standard
target_compile_features(Engine PUBLIC c_std_11)
target_compile_features(Engine PUBLIC cxx_std_17)

# Warnings
if(MSVC)
	target_compile_options(Engine PRIVATE /W4)
else()
	target_compile_options(Engine PRIVATE -Wall -Wextra)
endif()

# Multithreaded compilation.
if(MSVC AND NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Not allowed on Clang-cl for Visual Studio.
	target_compile_options(Engine PRIVATE /MP)
endif()

# Exceptions handling.
if(MSVC)
	target_compile_options(Engine PRIVATE /EHsc)
else()
	target_compile_options(Engine PRIVATE -fexceptions)
endif()

# Advanced MSVC preprocessor required for Core::Debug
# https://docs.microsoft.com/en-us/cpp/preprocessor/preprocessor-experimental-overview
if(MSVC AND NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")  # Not allowed on Clang-cl for Visual Studio.
	target_compile_options(Engine PUBLIC /experimental:preprocessor)
endif()



# === Build Configs ===

# Debug config.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

	message("Build config: Debug")

	# Disable code optimization.
	if(MSVC)
		target_compile_options(Engine PRIVATE /Od)
	else()
		target_compile_options(Engine PRIVATE -Og)
	endif()

	# Create multithreaded debug DLL.
	if(MSVC)
		target_compile_options(Engine PRIVATE /MDd)
	endif()

	# Add Debug information.
	if(MSVC)
		target_compile_options(Engine PRIVATE /Zi)

		if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Not allowed on Clang-cl for Visual Studio.)
			target_compile_options(Engine PRIVATE /Zf)
		endif()
	else()
		target_compile_options(Engine PRIVATE -g)
	endif()

	# Enables run-time error checking.
	if(MSVC)
		target_compile_options(Engine PRIVATE /RTC1)
	endif()


# Release config.
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	
	message("Build config: Release")

	# Disable RTTI: dynamic_cast<> and typeid. Use Sapphire reflection instead.
	if(MSVC)
		target_compile_options(Engine PRIVATE /GR-)
	else()
		target_compile_options(Engine PRIVATE -fno-rtti)
	endif()

	# Maximum code optimization: O2. O3 may be unstable and build undefined behavior.
	if(MSVC)
		target_compile_options(Engine PRIVATE /O2)
	else()
		target_compile_options(Engine PRIVATE -O2)
	endif()

	# Create multithreaded DLL.
	if(MSVC)
		target_compile_options(Engine PRIVATE /MD)
	endif()

else()

	message(FATAL_ERROR "Build type: ${CMAKE_BUILD_TYPE} not supported!")

endif()



# === Platforms ===

message("Platform: ${CMAKE_SYSTEM_NAME}")



# === Architecture ===

message("Architecture:  ${CMAKE_HOST_SYSTEM_PROCESSOR}")

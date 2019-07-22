# Copyright 2020 Sapphire development team. All Rights Reserved.

cmake_minimum_required(VERSION 3.12)


# === Projects ===

project(Sapphire)

message("Main directory: ${CMAKE_SOURCE_DIR}")


# === Build Configs ===

# Release config.
function(setup_release_config)
endfunction()

# Debug config.
function(setup_debug_config)

	# Default Debug preprocessor.
	add_compile_definitions(_DEBUG=1)

endfunction()

# Setup build config.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	
	message("Build config: Release")

	setup_release_config()

elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

	message("Build config: Debug")

	setup_debug_config()

else()

	message(FATAL_ERROR "Build type: ${CMAKE_BUILD_TYPE} not supported!")

endif()


# === Architecture ===

message("Architecture:  ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# x64 architecture.
function(setup_x64_architecture)

	# Default x64 preprocessor.
	add_compile_definitions(_x64=1)

endfunction()

# Arm architecture.
function(setup_arm_architecture)

	# Default arm preprocessor.
	add_compile_definitions(_ARM=1)

endfunction()


# Setup architecture.
if("${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "64")

	setup_x64_architecture()

endif()

if("${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "arm")

	setup_arm_architecture()

endif()


# === Platforms ===

# Windows platform.
function(setup_windows_platform)

	message("Platform: Windows")

endfunction()

# Android platform.
function(setup_android_platform)

	message("Platform: Android")

endfunction()

# Unix platform.
function(setup_unix_platform)

	message("Platform: Unix")

endfunction()

# MacOS platform.
function(setup_apple_platform)

	message("Platform: MacOS")

endfunction()

# Setup OS platform.
if (WIN32)

    setup_windows_platform()

elseif(ANDROID)

    setup_android_platform()

elseif(UNIX)

    setup_unix_platform()

elseif(APPLE)

    setup_apple_platform()

else()

	message(FATAL_ERROR "Platform not supported!")

endif()


# === Compilers ===

message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Compiler flags summary:
#	MSVC					GNU
#							-std=c1x			Set C standard version.
#	/std:c++latest			-std=c++1z			Set C++ standard version.
#	/W4						-W4					Enable warnings.
#	/O2						-O2					Maximum code optimization.
#	/Od						-Od -Og				Disable code optimization.
#	/RTC										Enables run-time error checking.
#	/MP						-j8					Multithreading compilation.
#	/EHsc					-fexceptions		Exceptions handling.
#	/GR-					-fno-rtti			Disable RTTI: dynamic_cast<> and typeid. Use Sapphire reflection instead.
#							-pedantic			Reject programs which use forbidden extentions.
#	/MD /MDd									Create multithreaded DLL ('d' with debug infos).
#	/Zi /Zf					-g					Add Debug information.
#
# References:
# https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically

# Clang compiler.
function(setup_clang_compiler)

	# Use Clang-cl for Visual Studio
	if(MSVC)

		# Default flags.
		set(CMAKE_C_FLAGS "/W4" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS "/W4 /EHsc /GR-" PARENT_SCOPE)

		# Release only flags (Require ${CMAKE_BUILD_TYPE} == "Release").
		set(CMAKE_C_FLAGS_RELEASE "/O2 /MD" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MD" PARENT_SCOPE)

		# Debug only flags (Require ${CMAKE_BUILD_TYPE} == "Debug").
		set(CMAKE_C_FLAGS_DEBUG "/Od /MDd /RTC /Zi" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS_DEBUG "/Od /MDd /RTC /Zi" PARENT_SCOPE)

	else()

		# Default flags.
		set(CMAKE_C_FLAGS "-Wall -Wextra -fexceptions -fno-rtti -pedantic" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -fexceptions -fno-rtti -pedantic" PARENT_SCOPE)

		# Release only flags (Require ${CMAKE_BUILD_TYPE} == "Release").
		set(CMAKE_C_FLAGS_RELEASE "-O2" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS_RELEASE "-O2" PARENT_SCOPE)

		# Debug only flags (Require ${CMAKE_BUILD_TYPE} == "Debug").
		set(CMAKE_C_FLAGS_DEBUG "-Og -g" PARENT_SCOPE)
		set(CMAKE_CXX_FLAGS_DEBUG "-Og -g" PARENT_SCOPE)

	endif()

endfunction()

# MSVC compiler.
function(setup_msvc_compiler)

	# Default flags.
	set(CMAKE_C_FLAGS "/W4 /MP" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS "/W4 /MP /EHsc /GR-" PARENT_SCOPE)

	# Release only flags (Require ${CMAKE_BUILD_TYPE} == "Release").
	set(CMAKE_C_FLAGS_RELEASE "/O2 /MD" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MD" PARENT_SCOPE)

	# Debug only flags (Require ${CMAKE_BUILD_TYPE} == "Debug").
	set(CMAKE_C_FLAGS_DEBUG "/Od /MDd /RTC1 /Zi /Zf" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS_DEBUG "/Od /MDd /RTC1 /Zi /Zf" PARENT_SCOPE)

endfunction()

# GNU compiler.
function(setup_gnu_compiler)

	# Default flags.
	set(CMAKE_C_FLAGS "-Wall -Wextra -fexceptions -fno-rtti -pedantic" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -fexceptions -fno-rtti -pedantic" PARENT_SCOPE)

	# Release only flags (Require ${CMAKE_BUILD_TYPE} == "Release").
	set(CMAKE_C_FLAGS_RELEASE "-O2" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS_RELEASE "-O2" PARENT_SCOPE)

	# Debug only flags (Require ${CMAKE_BUILD_TYPE} == "Debug").
	set(CMAKE_C_FLAGS_DEBUG "-Og -g" PARENT_SCOPE)
	set(CMAKE_CXX_FLAGS_DEBUG "-Og -g" PARENT_SCOPE)

endfunction()

# Setup compiler.
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

	setup_clang_compiler()

elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")

	setup_msvc_compiler()

elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

	setup_gnu_compiler()

else()

	message(FATAL_ERROR "Compiler: ${CMAKE_CXX_COMPILER_ID} not supported! Please use either Clang, MSVC or GNU compiler.")

endif()


# === Standards ===

# C Standard.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
message("C Standard: ${CMAKE_C_STANDARD}")

# C++ Standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message("C++ Standard: ${CMAKE_CXX_STANDARD}")


# === Outputs ===

# Setup output directories for project.
set(NEW_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR}")

# .lib / .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NEW_OUTPUT_DIR}")

# .dll / .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NEW_OUTPUT_DIR}")

# .exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NEW_OUTPUT_DIR}")

message("Output Directory: ${NEW_OUTPUT_DIR}")


# === Tests ===

enable_testing()

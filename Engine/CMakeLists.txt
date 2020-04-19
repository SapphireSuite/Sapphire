# Copyright 2020 Sapphire development team. All Rights Reserved.



# === Projects ===

project(Engine)



# === Inputs ===

# Parse files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*")

# Add dynamic library target.
add_library(Engine SHARED ${SOURCES})



# === Include directories ===

target_include_directories(Engine PUBLIC Include)
target_include_directories(Engine PUBLIC Include/Sapphire)



# === Dependencies ===

# Enable Multithreading (-pthread).
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Engine Threads::Threads)



# === Compile features ===

# Standard
target_compile_features(Engine PUBLIC c_std_11)
target_compile_features(Engine PUBLIC cxx_std_17)

# Warnings
if(MSVC)
	target_compile_options(Engine PUBLIC /W4)
else()
	target_compile_options(Engine PUBLIC -Wall -Wextra)
endif()

# Multithreaded compilation.
if(MSVC AND NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Not allowed on Clang-cl for Visual Studio.
	target_compile_options(Engine PUBLIC /MP)
endif()

# Exceptions handling.
if(MSVC)
	target_compile_options(Engine PUBLIC /EHsc)
else()
	target_compile_options(Engine PUBLIC -fexceptions)
endif()

# Disable RTTI: dynamic_cast<> and typeid. Use Sapphire reflection instead.
if(MSVC)
	target_compile_options(Engine PUBLIC /GR-)
else()
	target_compile_options(Engine PUBLIC -fno-rtti)
endif()



# === Build Configs ===

# Debug config.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

	message("Build config: Debug")

	# Disable code optimization.
	if(MSVC)
		target_compile_options(Engine PUBLIC /Od)
	else()
		target_compile_options(Engine PUBLIC -Og)
	endif()

	# Create multithreaded debug DLL.
	if(MSVC)
		target_compile_options(Engine PRIVATE /MDd)
	endif()

	# Add Debug information.
	if(MSVC)
		target_compile_options(Engine PUBLIC /Zi)

		if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Not allowed on Clang-cl for Visual Studio.)
			target_compile_options(Engine PUBLIC /Zf)
		endif()
	else()
		target_compile_options(Engine PUBLIC -g)
	endif()

	# Enables run-time error checking.
	if(MSVC)
		target_compile_options(Engine PUBLIC /RTC1)
	endif()


# Release config.
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	
	message("Build config: Release")

	# Maximum code optimization: O2. O3 may be unstable and build undefined behavior.
	if(MSVC)
		target_compile_options(Engine PUBLIC /O2)
	else()
		target_compile_options(Engine PUBLIC -O2)
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

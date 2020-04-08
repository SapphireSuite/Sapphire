# Copyright 2020 Sapphire development team. All Rights Reserved.


# === Projects ===

project(UnitTest)


# === Inputs ===

# Parse cpp files.
file(GLOB_RECURSE SOURCES "*")

# Add executable target.
add_executable(UnitTest ${SOURCES})


# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTest PRIVATE Engine)


# === Tests ===

add_test(NAME CUnitTests COMMAND UnitTest --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTest>)

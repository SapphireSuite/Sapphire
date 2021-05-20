# Copyright 2020 Sapphire development team. All Rights Reserved.


# === Projects ===

project(Prototype)


# === Inputs ===

# Parse cpp files.
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# Add executable target.
add_executable(Prototype ${SOURCES})


# === Dependencies ===

# Add library dependencies.
target_link_libraries(Prototype PRIVATE Engine)


# === Tests ===

add_test(NAME CPrototype COMMAND Prototype --config $<CONFIGURATION> --exe $<TARGET_FILE:Prototype>)
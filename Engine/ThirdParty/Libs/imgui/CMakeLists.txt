# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SOURCES *)

# Incluez les sous-projets.
add_library(imgui INTERFACE)

target_sources(imgui INTERFACE ${SOURCES})
target_include_directories(imgui INTERFACE	${CMAKE_CURRENT_SOURCE_DIR})

# glfw dependancy
target_include_directories(imgui INTERFACE	"${CMAKE_CURRENT_SOURCE_DIR}/../glfw-3.3/include")


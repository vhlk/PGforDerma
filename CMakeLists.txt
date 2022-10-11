# CMakeList.txt: arquivo de projeto de CMake de nível superior, fazer configuração global
# e inclua subprojetos aqui.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

project ("PGforDerma")

# Inclua subprojetos.
add_subdirectory ("PGforDerma")

target_compile_definitions(PGforDerma PUBLIC ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
# CMakeList.txt: projeto de CMake para PGforDerma, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Adicione a origem ao executável deste projeto.
add_executable (PGforDerma "PGforDerma.cpp" "PGforDerma.hpp" "RandomTree.hpp" "RandomNode.hpp" "RandomNode.cpp" "RandomTree.cpp")
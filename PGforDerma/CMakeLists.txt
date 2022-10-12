# CMakeList.txt: projeto de CMake para PGforDerma, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# DataFrame (csv reader)
include(ExternalProject)
ExternalProject_Add(DataFrame
  URL https://github.com/hosseinmoein/DataFrame/archive/refs/tags/1.21.0.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/DataFrame
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/DataFrame.lib
)

ExternalProject_Get_Property(DataFrame install_dir)


include_directories(SYSTEM ${install_dir}/include)

# Adicione a origem ao executável deste projeto.
add_executable (PGforDerma "PGforDerma.cpp" "PGforDerma.hpp" "RandomTree.hpp" "RandomNode.hpp" "RandomNode.cpp" "RandomTree.cpp")

add_dependencies(PGforDerma DataFrame)

target_link_libraries(PGforDerma ${install_dir}/lib/DataFrame.lib)
// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomNode.hpp"
#include "RandomTree.hpp"
#include <array>

int main()
{
	auto tree = std::make_unique<RandomTree>(0.1, 1);

	std::cout << "printing tree..." << std::endl;
	
	tree->print();
}

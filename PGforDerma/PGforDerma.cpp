// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomNode.hpp"
#include "RandomTree.hpp"
#include <array>
#include <random>

int main()
{
	auto tree = std::make_unique<RandomTree>(0.4, 0.4);

	std::cout << "printing tree...\n" << std::endl;
	
	tree->print();
}
// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomNode.hpp"
#include "RandomTree.hpp"
#include <array>
#include <random>
#include <DataFrame/DataFrame.h>


int main()
{
	using namespace hmdf;
	using StrDataFrame = StdDataFrame<int>;

	StrDataFrame ibm_df;
	ibm_df.read((std::string(ROOT_DIR) + "/dataset/Dermatology_new.csv").c_str(), io_format::csv2);
	std::cout << ibm_df.col_idx_to_name(0) << ',' << ibm_df.col_idx_to_name(1) << ',' << ibm_df.col_idx_to_name(2) << std::endl;

	
	auto tree = std::make_unique<RandomTree>(0.4, 0.4);

	std::cout << "printing tree...\n" << std::endl;
	
	tree->print();
}
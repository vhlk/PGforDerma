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

	
	auto tree = std::make_unique<RandomTree>(0.65, 0.65);
	std::cout << std::endl << "printing tree...(seed: " << tree->get_seed() << ")\n" << std::endl;
	//tree->print();
	std::cout << "num of nodes: " << tree->get_number_nodes() << std::endl;

	auto [num_rows, num_cols] = ibm_df.shape();
	
	std::vector<std::vector<int>> X;
	std::vector<int> y;

	for (int i = 0; i < num_rows; i++) {
		auto row = ibm_df.get_row<int>(i);

		auto Xi = row.get_vector<int>();

		y.push_back(Xi.back());

		Xi.erase(Xi.begin()); // remove index
		Xi.pop_back(); // remove y

		X.push_back(Xi);
	}

	std::cout << "Accuracy: " << tree->get_accuracy(X, y);

	tree = std::make_unique<RandomTree>(0.65, 0.65);
	std::cout << std::endl << "printing second tree...(seed: " << tree->get_seed() << ")\n" << std::endl;
	//tree->print();
	std::cout << "num of nodes: " << tree->get_number_nodes() << std::endl;

	std::cin.get();
}
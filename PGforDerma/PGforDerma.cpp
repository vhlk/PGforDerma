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


void print_node(const RandomNode* node, int level) {
	Conversions conversions;

	if (node != nullptr) {
		print_node(node->left.get(), level + 1);

		std::string spacing(8 * level, ' ');

		if (node->has_target()) {
			std::cout << spacing << "-> " << conversions.get_target_name(node->get_target()) << std::endl;
		}
		else {
			std::string comparison = "(" + std::to_string(node->get_feature()) + " " +
				conversions.get_string_comparator(node->get_comparator()) + " " + std::to_string(node->get_comparating_value()) + ")";

			std::cout << spacing << "-> " << comparison << std::endl;
		}

		print_node(node->right.get(), level + 1);
	}
}

int main()
{
	auto node = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left->left = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left->left->left = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->right = std::make_unique<RandomNode>(std::vector<int>(), false);
	std::cout << "Node:" << std::endl;
	print_node(node.get(), 0);

	auto node_copy = node->copy();
	std::cout << "Node copy:" << std::endl;
	print_node(node_copy.get(), 0);

	auto node_s_copy = node->shallow_copy();
	std::cout << "Node shallow copy:" << std::endl;
	print_node(node_s_copy.get(), 0);

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

		// keeping index since features starts at index 1
		// Xi.erase(Xi.begin()); // remove index
		Xi.pop_back(); // remove y

		X.push_back(Xi);
	}

	std::cout << "Accuracy: " << tree->get_accuracy(X, y) << std::endl;

	tree->mutate_target();
	std::cout << "Accuracy after mutation: " << tree->get_accuracy(X, y) << std::endl;


	tree = std::make_unique<RandomTree>(0.65, 0.65);
	std::cout << std::endl << "printing second tree...(seed: " << tree->get_seed() << ")\n" << std::endl;
	//tree->print();
	std::cout << "num of nodes: " << tree->get_number_nodes() << std::endl;

	std::cin.get();
}
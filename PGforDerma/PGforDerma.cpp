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


void print_node(const RandomNode* node, int level=0) {
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

void switch_nodes(RandomNode* node1, RandomNode* node2, bool go_left1, bool go_left2) {
	if (node1->has_target() || node2->has_target()) {
		auto tmp = node1->copy();
		node1->copy_from(node2->copy()); // any better way to convert node2 from pointer to unique ptr?
		node2->copy_from(tmp);
	}
	else {
		auto& tmp_ptr_1 = go_left1 ? node1->left : node1->right;
		auto tmp_ptr1_copy = tmp_ptr_1->copy();
		auto& tmp_ptr_2 = go_left2 ? node2->left : node2->right;

		tmp_ptr_1 = std::move(tmp_ptr_2);
		tmp_ptr_2 = std::move(tmp_ptr1_copy);
	}
}

int main()
{
	auto my_tree_gostosa = std::make_unique<RandomTree>(1, 1, 10);
	my_tree_gostosa->print();
	std::cout << "Num nodes: " << my_tree_gostosa->get_number_nodes() << std::endl << std::endl;
	auto my_tree_gostosona = std::make_unique<RandomTree>(1, 1, 0, 5);
	my_tree_gostosona->print();
	std::cout << "Depth: " << my_tree_gostosona->get_depth() << std::endl << std::endl;

	auto node = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left->left = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->left->right = std::make_unique<RandomNode>(std::vector<int>(), true);
	node->left->left->left = std::make_unique<RandomNode>(std::vector<int>(), true);
	node->left->left->right = std::make_unique<RandomNode>(std::vector<int>(), true);
	node->right = std::make_unique<RandomNode>(std::vector<int>(), false);
	node->right->right = std::make_unique<RandomNode>(std::vector<int>(), true);
	node->right->left = std::make_unique<RandomNode>(std::vector<int>(), true);
	std::cout << "Node:" << std::endl;
	print_node(node.get(), 0);

	auto node_copy = node->copy();
	std::cout << "Node copy:" << std::endl;
	print_node(node_copy.get());

	auto node_s_copy = node->shallow_copy();
	std::cout << "Node shallow copy:" << std::endl;
	print_node(node_s_copy.get());

	std::cout << "Node after switch:" << std::endl;
	switch_nodes(node->left.get(), node->right.get(), true, false);
	print_node(node.get());

	using namespace hmdf;
	using StrDataFrame = StdDataFrame<int>;

	StrDataFrame ibm_df;
	ibm_df.read((std::string(ROOT_DIR) + "/dataset/Dermatology_new.csv").c_str(), io_format::csv2);
	std::cout << ibm_df.col_idx_to_name(0) << ',' << ibm_df.col_idx_to_name(1) << ',' << ibm_df.col_idx_to_name(2) << std::endl;

	
	auto tree = std::make_unique<RandomTree>(0.65, 0.65, 50, 10);
	//std::cout << std::endl << "printing tree...(seed: " << tree->get_seed() << ")\n" << std::endl;
	//tree->print();
	//std::cout << std::endl;
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
	std::cout << std::endl << "printing tree after mutation...\n" << std::endl;
	tree->print();
	std::cout << std::endl;
	std::cout << "Accuracy after mutation: " << tree->get_accuracy(X, y) << std::endl;


	auto _ = std::make_unique<RandomTree>(0, 0);
	_ = std::make_unique<RandomTree>(0, 0);
	_ = std::make_unique<RandomTree>(0, 0);

	auto tree1 = std::make_unique<RandomTree>(0.6, 0.6, 50, 10);
	std::cout << std::endl << "printing second tree...(seed: " << tree1->get_seed() << ")\n" << std::endl;
	tree1->print();
	std::cout << std::endl;
	std::cout << "num of nodes: " << tree1->get_number_nodes() << std::endl;


	std::cout << "Trying to recombine..." << std::endl;
	auto [son1,son2] = tree->recombine(tree1, 0.1);
	std::cout << "Son 1:" << std::endl;
	son1->print();
	std::cout << "Son 2: " << std::endl;
	son2->print();

	std::cin.get();
}
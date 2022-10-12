#pragma once
#include <iostream>
#include <tuple>
#include <optional>
#include <random>
#include "RandomNode.hpp"

class RandomTree {
public:
	RandomTree(double left_prob, double right_prob);

	void print();

	constexpr int get_seed() const { return seed;  }

	int get_number_nodes() const { return get_number_nodes(root.get()); }
private:
	std::unique_ptr<RandomNode> root;

	bool try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);
	void insert_random_left(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);
	void insert_random_right(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);

	std::tuple<std::unique_ptr<RandomNode>, std::optional<std::vector<int>>> create_new_node(const std::vector<int>& seen_features_in_branch);
	void print_tree(const RandomNode* node, int level);

	TreeSeed* tree_seed_singleton;
	const int seed = tree_seed_singleton->getInstance().get_seed();
	std::mt19937 gen = std::mt19937(seed);

	int get_number_nodes(const RandomNode* curr_node) const;
};
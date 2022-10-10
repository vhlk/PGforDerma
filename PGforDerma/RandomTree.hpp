#pragma once
#include <iostream>
#include "RandomNode.hpp"
#include <tuple>
#include <optional>

class RandomTree {
public:
	RandomTree(double left_prob, double right_prob);

	void print();

private:
	std::unique_ptr<RandomNode> root;

	bool try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);
	bool insert_random_left(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);
	bool insert_random_right(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch);

	std::tuple<std::unique_ptr<RandomNode>, std::optional<std::vector<int>>> create_new_node(const std::vector<int>& seen_features_in_branch);
	void print_tree(const std::string& prefix, const RandomNode* node, bool isLeft);
};
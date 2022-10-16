#pragma once
#include <iostream>
#include <tuple>
#include <random>
#include "RandomNode.hpp"
#include "Utils.hpp"

class RandomTree {
public:
	RandomTree(double left_prob, double right_prob, int max_nodes = 0, int max_depth = 0);

	void print() const;
	std::string predict(const std::vector<int>& X) { return predict(root, X); };
	double get_accuracy(const std::vector<std::vector<int>>& X, const std::vector<int>& y);

	void mutate_target() { mutate_target(root); };
	std::tuple<std::unique_ptr<RandomTree>, std::unique_ptr<RandomTree>> recombine(const std::unique_ptr<RandomTree>& other, double stddev_percent);

	constexpr int get_seed() const { return seed; }

	int get_depth() const { return get_depth(root); };
	int get_number_nodes() const { return get_number_nodes(root.get()); }

	~RandomTree() { delete conversions; delete comparations; }
private:
	std::unique_ptr<RandomNode> root;

	void mutate_target(std::unique_ptr<RandomNode>& node);

	std::string predict(const std::unique_ptr<RandomNode>& node, const std::vector<int>& X) const;

	void try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch, int max_nodes = 0, int max_depth = 0);

	std::unique_ptr<RandomNode> create_new_node(const std::vector<int>& seen_features_in_branch) const;
	constexpr void print_tree(const RandomNode* node, int level) const;

	const TreeSeed* tree_seed_singleton;
	const int seed = tree_seed_singleton->getInstance().get_seed();
	std::mt19937 gen = std::mt19937(seed);

	int get_depth(const std::unique_ptr<RandomNode>& node, int curr_depth = 0) const;
	int get_number_nodes(const RandomNode* curr_node) const;

	std::unique_ptr<RandomTree> node_to_tree(std::unique_ptr<RandomNode> node);
	void switch_nodes(RandomNode* node1, RandomNode* node2, bool go_left1, bool go_left2) const;

	Conversions* conversions = new Conversions();
	Comparations* comparations = new Comparations();
};
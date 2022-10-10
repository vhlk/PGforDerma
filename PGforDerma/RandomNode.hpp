#pragma once
#include <iostream>
#include <vector>
#include "Definitions.hpp"

constexpr int seed = 2703;

class RandomNode {
public:
	RandomNode(const std::vector<int>& seen_features_in_branch, bool is_leaf_node); // creates a Node with random values

	std::unique_ptr<RandomNode> left;
	std::unique_ptr<RandomNode> right;

	std::vector<int> seen_features_in_branch;

	void if_not_set_target();
	constexpr int get_comparator() const { return comparator; }
	constexpr void set_comparator(const int comparator) { this->comparator = comparator; }
	constexpr int get_feature() const { return feature; }
	constexpr void set_feature(const int feature) { this->feature = feature; }
	constexpr int get_comparating_value() const { return comparating_value; }
	constexpr void set_comparating_value(const int comparating_value) { this->comparating_value = comparating_value; }
	constexpr bool has_target() const { return target != -1; }
	constexpr int get_target() const { return target; }
	constexpr void set_target(const int target) { this->target = target; }
private:
	void get_random_target();	

	int comparator; // [">", "<", ">=", "<=", "!=", "=="]
	int feature; // from 'erythema' to 'Age'
	int comparating_value; // [0 .. 3], except for family history (0 or 1)
	int target = -1; // dissease ( [ 1 .. 7 ] )
};
#include "RandomNode.hpp"

RandomNode::RandomNode(const std::vector<int>& seen_features_in_branch, bool is_leaf_node) {
	if (is_leaf_node) {
		get_random_target();
		return;
	}

	PossibleValues possible_values;
	Conversions conversions;

	this->seen_features_in_branch = seen_features_in_branch;

	// get all possible features remaining
	auto features = possible_values.features;
	for (auto feat : seen_features_in_branch)
		features.erase(std::remove(features.begin(), features.end(), feat));

	if (features.size() == 0) {
		get_random_target();
		return;
	}

	// get the feature for this node
	feature = get_random_elem(features);

	// get comparator
	auto& comparators = conversions.get_feature_name(feature) == "Age" ? possible_values.comparators_age : possible_values.comparators;
	comparator = get_random_elem(comparators);

	// get comparating (to) value
	auto& comparating_values = conversions.get_feature_name(feature) == "Age" ? possible_values.values_age :
		conversions.get_feature_name(feature) == "Family History" ? possible_values.values_hist_familiar : possible_values.values;

	comparating_value = get_random_elem(comparating_values);
}

void RandomNode::if_not_set_target() {
	if (!has_target()) {
		get_random_target();
	}
}

std::unique_ptr<RandomNode> RandomNode::copy() const {
	auto new_node = shallow_copy();

	if (left)
		new_node->left = left->copy();
	if (right)
		new_node->right = right->copy();

	return new_node;
}

std::unique_ptr<RandomNode> RandomNode::shallow_copy() const {
	auto new_node = std::make_unique<RandomNode>(seen_features_in_branch, has_target());
	new_node->set_comparator(comparator);
	new_node->set_feature(feature);
	new_node->set_comparating_value(comparating_value);
	new_node->set_target(target);
	new_node->seen_features_in_branch = seen_features_in_branch;

	return new_node;
}

void RandomNode::copy_from(const std::unique_ptr<RandomNode>& node_to_be_copied) {
	comparator = node_to_be_copied->get_comparator();
	feature = node_to_be_copied->get_feature();
	comparating_value = node_to_be_copied->get_comparating_value();
	target = node_to_be_copied->get_target();
	seen_features_in_branch = node_to_be_copied->seen_features_in_branch;

	if (node_to_be_copied->left)
		left = node_to_be_copied->left->copy();
	else if (left)
		left.reset();

	if (node_to_be_copied->right)
		right = node_to_be_copied->right->copy();
	else if (right)
		right.reset();
}

void RandomNode::get_random_target() {
	PossibleValues possible_values;

	auto& targets = possible_values.targets;
	target = get_random_elem(targets);
}

template <typename T> 
T RandomNode::get_random_elem(const std::vector<T>& in) {
	std::uniform_int_distribution<> distr(0, (int)in.size() - 1);
	return in.at(distr(gen));
}

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

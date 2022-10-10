#include "RandomNode.hpp"
#include <random>

RandomNode::RandomNode(const std::vector<int>& seen_features_in_branch, bool is_leaf_node) {
	if (is_leaf_node) {
		get_random_target();
		return;
	}

	PossibleValues possible_values;
	Conversions conversions;

	this->seen_features_in_branch = seen_features_in_branch;

	std::mt19937 gen(seed);

	// get all possible features remaining
	auto features = possible_values.features;
	for (auto feat : seen_features_in_branch)
		features.erase(std::remove(features.begin(), features.end(), feat));

	if (features.size() == 0) {
		get_random_target();
		return;
	}

	// get the feature for this node
	std::uniform_int_distribution<> distr(0, (int) features.size() - 1);
	feature = features.at(distr(gen));

	// get comparator
	if (conversions.get_feature_name(feature) == "Age") {
		auto& comparator_ages = possible_values.comparators_age;
		std::uniform_int_distribution<> distr(0, (int) comparator_ages.size() - 1);
		comparator = comparator_ages.at(distr(gen));
	}
	else {
		auto& comparators = possible_values.comparators;
		std::uniform_int_distribution<> distr(0, (int) comparators.size() - 1);
		comparator = comparators.at(distr(gen));
	}

	// get comparating (to) value
	if (conversions.get_feature_name(feature) == "Age") {
		auto& comparating_values = possible_values.values_age;
		std::uniform_int_distribution<> distr(0, (int) comparating_values.size() - 1);
		comparating_value = comparating_values.at(distr(gen));
	}
	else if (conversions.get_feature_name(feature) == "Family History") {
		auto& comparating_values = possible_values.values_hist_familiar;
		std::uniform_int_distribution<> distr(0, (int) comparating_values.size() - 1);
		comparating_value = comparating_values.at(distr(gen));
	}
	else {
		auto& comparating_values = possible_values.values;
		std::uniform_int_distribution<> distr(0, (int) comparating_values.size() - 1);
		comparating_value = comparating_values.at(distr(gen));
	}
}

void RandomNode::if_not_set_target() {
	if (!has_target()) {
		get_random_target();
	}
}

void RandomNode::get_random_target() {
	PossibleValues possible_values;
	std::mt19937 gen(seed);

	auto& targets = possible_values.targets;
	std::uniform_int_distribution<> distr(0, (int) targets.size() - 1);
	target = targets.at(distr(gen));
}

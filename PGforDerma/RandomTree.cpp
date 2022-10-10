#include "RandomTree.hpp"
#include <random>

RandomTree::RandomTree(double left_prob, double right_prob) {
	root = std::make_unique<RandomNode>(std::vector<int>{}, false);

	bool has_inserted = try_insert(root, left_prob, right_prob, {});

	if (!has_inserted) {
		root->if_not_set_target();
	}
}

void RandomTree::print() {
	print_tree("", root.get(), false);
}

bool RandomTree::try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch)
{
	std::mt19937 gen(seed);
	std::uniform_real_distribution<> distr(0, 1);

	bool has_inserted = false;

	if (distr(gen) <= left_prob) {
		insert_random_left(root, left_prob, right_prob, seen_features_in_branch);
		has_inserted = true;
	}
	if (distr(gen) <= right_prob) {
		insert_random_right(root, left_prob, right_prob, seen_features_in_branch);
		has_inserted = true;
	}

	return has_inserted;
}

bool RandomTree::insert_random_left(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch) {
	auto [new_node, new_features_branch] = create_new_node(seen_features_in_branch);

	node->left = std::move(new_node);

	if (!new_features_branch.has_value())
		return false;

	return try_insert(node->left, left_prob, right_prob, new_features_branch.value());
}

bool RandomTree::insert_random_right(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch) {
	auto [new_node, new_features_branch] = create_new_node(seen_features_in_branch);

	node->right = std::move(new_node);

	if (!new_features_branch.has_value())
		return false;

	return try_insert(node->right, left_prob, right_prob, new_features_branch.value());
}

std::tuple<std::unique_ptr<RandomNode>, std::optional<std::vector<int>>> RandomTree::create_new_node(const std::vector<int>& seen_features_in_branch)
{
	auto new_node = std::make_unique<RandomNode>(seen_features_in_branch, false);

	if (new_node->has_target())
		return { std::move(new_node), std::nullopt };

	std::vector<int> new_features_branch(seen_features_in_branch);
	new_features_branch.push_back(new_node->get_feature());

	return  { std::move(new_node), new_features_branch };
}

void RandomTree::print_tree(const std::string& prefix, const RandomNode* node, bool isLeft) {
	Conversions conversion;

	if (node != nullptr)
	{
		std::cout << prefix;

		std::cout << (isLeft ? "L--" : "|--");

		// print the value of the node
		if (node->has_target())
			std::cout << node->get_target() << std::endl;
		else
			std::cout << '(' << node->get_feature() << ' ' << conversion.get_string_comparator(node->get_comparator()) << " " << node->get_comparating_value() << ')' << std::endl;

		// enter the next tree level - left and right branch
		print_tree(prefix + (isLeft ? "|   " : "    "), node->left.get(), true);
		print_tree(prefix + (isLeft ? "|   " : "    "), node->right.get(), false);
	}
}

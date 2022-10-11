#include "RandomTree.hpp"
#include <string>

RandomTree::RandomTree(double left_prob, double right_prob) {
	root = std::make_unique<RandomNode>(std::vector<int>{}, false);

	bool has_inserted = try_insert(root, left_prob, right_prob, { root->get_feature() });

	if (!has_inserted) {
		root->if_not_set_target();
	}
}

void RandomTree::print() {
	print_tree(root.get(), 0);
}

bool RandomTree::try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch)
{
	std::uniform_real_distribution<> distr(0, 1);

	bool has_inserted_left = false;
	bool has_inserted_right = false;

	if (distr(gen) <= left_prob) {
		insert_random_left(node, left_prob, right_prob, seen_features_in_branch);
		has_inserted_left = true;
	}
	if (distr(gen) <= right_prob) {
		insert_random_right(node, left_prob, right_prob, seen_features_in_branch);
		has_inserted_right = true;
	}

	if (!has_inserted_left && has_inserted_right)
		node->left = std::make_unique<RandomNode>(seen_features_in_branch, true);
	if (!has_inserted_right && has_inserted_left)
		node->right = std::make_unique<RandomNode>(seen_features_in_branch, true);

	return has_inserted_left || has_inserted_right;
}

void RandomTree::insert_random_left(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch) {
	auto [new_node, new_features_branch] = create_new_node(seen_features_in_branch);

	node->left = std::move(new_node);

	if (!new_features_branch.has_value())
		return;

	bool inserted = try_insert(node->left, left_prob, right_prob, new_features_branch.value());
	
	if (!inserted)
		node->left->if_not_set_target();
}

void RandomTree::insert_random_right(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch) {
	auto [new_node, new_features_branch] = create_new_node(seen_features_in_branch);

	node->right = std::move(new_node);

	if (!new_features_branch.has_value())
		return;

	bool inserted = try_insert(node->right, left_prob, right_prob, new_features_branch.value());
	
	if (!inserted)
		node->right->if_not_set_target();
}

std::tuple<std::unique_ptr<RandomNode>, std::optional<std::vector<int>>> RandomTree::create_new_node(const std::vector<int>& seen_features_in_branch) {
	auto new_node = std::make_unique<RandomNode>(seen_features_in_branch, false);

	if (new_node->has_target())
		return { std::move(new_node), std::nullopt };

	std::vector<int> new_features_branch(seen_features_in_branch);
	new_features_branch.push_back(new_node->get_feature());

	return  { std::move(new_node), new_features_branch };
}

//void RandomTree::print_tree(const std::string& prefix, const RandomNode* node, bool isLeft) {
//	Conversions conversion;
//
//	if (node != nullptr) {
//		std::cout << prefix;
//
//		std::cout << (isLeft ? "|--" : "L--");
//
//		// print the value of the node
//		if (node->has_target())
//			std::cout << node->get_target() << std::endl;
//		else
//			std::cout << '(' << node->get_feature() << ' ' << conversion.get_string_comparator(node->get_comparator()) << " " << node->get_comparating_value() << ')' << std::endl;
//
//		// enter the next tree level - left and right branch
//		print_tree(prefix + (isLeft ? "|   " : "    "), node->left.get(), true);
//		print_tree(prefix + (isLeft ? "|   " : "    "), node->right.get(), false);
//	}
//}

void RandomTree::print_tree(const RandomNode* node, int level) {
	Conversions conversion;

	if (node != nullptr) {
		print_tree(node->left.get(), level + 1);

		std::string spacing(8 * level, ' ');

		if (node->has_target()) {
			std::cout << spacing << "-> " << conversion.get_feature_name(node->get_target()) << std::endl;
		} else {
			std::string comparison = "(" + std::to_string(node->get_feature()) + " " + 
				conversion.get_string_comparator(node->get_comparator()) + " " + std::to_string(node->get_comparating_value()) + ")";

			std::cout << spacing << "-> " << comparison << std::endl;
		}

		print_tree(node->right.get(), level + 1);
	}
}

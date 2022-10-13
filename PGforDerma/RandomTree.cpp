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

double RandomTree::get_accuracy(const std::vector<std::vector<int>>& X, const std::vector<int>& y) {
	double correct_preds = 0;

	for (int i = 0; i < X.size(); i++) {
		if (predict(X[i]) == conversions->get_target_name(y[i]))
			correct_preds++;
	}

	return correct_preds / X.size();
}

void RandomTree::mutate_target(std::unique_ptr<RandomNode>& node) {
	if (node->has_target()) {
		node->mutate_target();
		return;
	}

	std::uniform_int_distribution<> distr(1, 10);

	if (distr(gen) <= 5)
		return mutate_target(node->left);
	else
		return mutate_target(node->right);
}

std::string RandomTree::predict(const std::unique_ptr<RandomNode>& node, const std::vector<int>& X) const {
	if (node->has_target())
		return conversions->get_target_name(node->get_target());

	if (comparations->compare(X[node->get_feature()], conversions->get_string_comparator(node->get_comparator()), node->get_comparating_value()))
		return predict(node->left, X);
	
	return predict(node->right, X);
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
	new_features_branch.shrink_to_fit();

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
	if (node != nullptr) {
		print_tree(node->left.get(), level + 1);

		std::string spacing(8 * level, ' ');

		if (node->has_target()) {
			std::cout << spacing << "-> " << conversions->get_target_name(node->get_target()) << std::endl;
		} else {
			std::string comparison = "(" + std::to_string(node->get_feature()) + " " + 
				conversions->get_string_comparator(node->get_comparator()) + " " + std::to_string(node->get_comparating_value()) + ")";

			std::cout << spacing << "-> " << comparison << std::endl;
		}

		print_tree(node->right.get(), level + 1);
	}
}

int RandomTree::get_number_nodes(const RandomNode* curr_node) const {
	if (curr_node->has_target())
		return 1;

	int res = 1;
	if (curr_node->left)
		res += get_number_nodes(curr_node->left.get());
	
	if (curr_node->right)
		res += get_number_nodes(curr_node->right.get());


	return res;
}
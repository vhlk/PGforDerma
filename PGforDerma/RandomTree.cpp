#include "RandomTree.hpp"
#include <string>
#include <queue>

RandomTree::RandomTree(double left_prob, double right_prob, int max_nodes, int max_depth) {
	if (max_depth < 0) throw std::invalid_argument("max_depth out of range: " + std::to_string(max_depth) + " (must be >= 0)");
	if (max_nodes < 0) throw std::invalid_argument("max_nodes out of range: " + std::to_string(max_nodes) + " (must be >= 0)");

	root = std::make_unique<RandomNode>(std::vector<int>{}, false);

	try_insert(root, left_prob, right_prob, { root->get_feature() }, max_nodes, max_depth);
}

void RandomTree::print() const {
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

std::unique_ptr<RandomTree> RandomTree::copy() {
	auto tree = std::make_unique<RandomTree>(0, 0, 1, 1);
	tree->root = root->copy();
	
	return tree;
}

std::pair<std::unique_ptr<RandomTree>, std::unique_ptr<RandomTree>> RandomTree::recombine(const std::unique_ptr<RandomTree>& other, double stddev_percent) {
	if (stddev_percent < 0 || stddev_percent > 1)
		throw std::invalid_argument("stddev_percent out of range: " + std::to_string(stddev_percent) + " (should be between 0.0 and 1.0)");

	std::uniform_int_distribution<> i_distr(1, 10);

	// copy is expensive, could this be optimized?
	auto tree1_root = root->copy();
	auto tree2_root = other->root->copy();

	int tree1_depth = get_depth();
	int tree2_depth = other->get_depth();
	int min_depth = std::min(tree1_depth, tree2_depth);
	double two_thirds_depth = 2 * min_depth / 3.0;

	std::normal_distribution<> n_distr(two_thirds_depth, min_depth * stddev_percent);

	int target_depth = (int)n_distr(gen);

	auto ptr1 = tree1_root.get();
	auto ptr2 = tree2_root.get();

	int curr_depth = 1;
	while (true) {
		bool ptr1_go_left = i_distr(gen) <= 5;
		bool ptr2_go_left = i_distr(gen) <= 5;


		if (curr_depth >= target_depth || ptr1->has_target() || ptr2->has_target()) {
			switch_nodes(ptr1, ptr2, ptr1_go_left, ptr2_go_left); // TODO: make sure there is no memmory leak
			return std::make_pair(std::move(node_to_tree(std::move(tree1_root))), std::move(node_to_tree(std::move(tree2_root))));
		}

		ptr1 = ptr1_go_left ? ptr1->left.get() : ptr1->right.get();
		ptr2 = ptr2_go_left ? ptr2->left.get() : ptr2->right.get();

		curr_depth++;
	}
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

void RandomTree::mutate_aux(std::unique_ptr<RandomNode>& node, double prob_finding_node, std::function<void(RandomTree*, std::unique_ptr<RandomNode>&)> mutate_fun) {
	if (node->has_target()) return;

	std::uniform_real_distribution<> r_distr(0, 1);

	bool mutate_this = r_distr(gen) <= prob_finding_node;

	if (!mutate_this) {
		bool try_mutate_left = r_distr(gen) < 0.5;

		if (try_mutate_left && !node->left->has_target())
			return mutate_aux(node->left, prob_finding_node, mutate_fun);

		if (!try_mutate_left && !node->right->has_target())
			return mutate_aux(node->right, prob_finding_node, mutate_fun);
	}

	mutate_fun(this, node);
}

void RandomTree::mutate_comparator_(std::unique_ptr<RandomNode>& node) {
	node->mutate_comparator();
}

void RandomTree::mutate_comparating_value_(std::unique_ptr<RandomNode>& node) {
	node->mutate_comparating_value();
}

std::string RandomTree::predict(const std::unique_ptr<RandomNode>& node, const std::vector<int>& X) const {
	if (node->has_target())
		return conversions->get_target_name(node->get_target());

	if (comparations->compare(X[node->get_feature()], conversions->get_string_comparator(node->get_comparator()), node->get_comparating_value()))
		return predict(node->left, X);
	
	return predict(node->right, X);
}

void RandomTree::try_insert(std::unique_ptr<RandomNode>& node, double left_prob, double right_prob, const std::vector<int>& seen_features_in_branch, int max_nodes, int max_depth) {
	std::uniform_real_distribution<> distr(0, 1);

	std::queue<std::pair<RandomNode*, bool>> border;
	border.push({ node.get(), true });

	int inserted_nodes = 1;
	int curr_depth = 1;

	bool pass_start_level = false;
	while (border.size() > 0 && (max_nodes == 0 || inserted_nodes < max_nodes) && (max_depth == 0 || curr_depth < max_depth)) {
		auto curr = border.front(); border.pop();

		if (curr.second)
			curr_depth++;

		bool has_inserted_left = false;
		bool has_inserted_right = false;

		if (distr(gen) <= left_prob) {
			auto new_node = create_new_node(curr.first->seen_features_in_branch);
			curr.first->left = std::move(new_node);

			if (!curr.first->left->has_target())
				border.push({ curr.first->left.get(), pass_start_level ? true : curr.second });

			has_inserted_left = true;
			inserted_nodes++;
		}
		if (distr(gen) <= right_prob) {
			auto new_node = create_new_node(curr.first->seen_features_in_branch);
			curr.first->right = std::move(new_node);

			if (!curr.first->right->has_target())
				border.push({ curr.first->right.get(), has_inserted_left ? false : pass_start_level ? true : curr.second });

			has_inserted_right = true;
			inserted_nodes++;
		}

		if (!has_inserted_left && has_inserted_right) {
			curr.first->left = std::make_unique<RandomNode>(seen_features_in_branch, true);
			inserted_nodes++;
		}
		if (!has_inserted_right && has_inserted_left) {
			curr.first->right = std::make_unique<RandomNode>(seen_features_in_branch, true);
			inserted_nodes++;
		}

		if (!has_inserted_left && !has_inserted_right) {
			if (curr.second)
				pass_start_level = true;

			curr.first->if_not_set_target();
		}
		else {
			pass_start_level = false;
		}
	}

	while (border.size() > 0) {
		auto curr = border.front(); border.pop();
		curr.first->if_not_set_target();
	}


}

std::unique_ptr<RandomNode> RandomTree::create_new_node(const std::vector<int>& seen_features_in_branch) const {
	auto new_node = std::make_unique<RandomNode>(seen_features_in_branch, false);

	std::vector<int> new_features_branch(seen_features_in_branch);
	new_features_branch.push_back(new_node->get_feature());
	new_features_branch.shrink_to_fit();

	new_node->seen_features_in_branch = new_features_branch;

	return  new_node;
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

constexpr void RandomTree::print_tree(const RandomNode* node, int level) const {
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

int RandomTree::get_depth(const std::unique_ptr<RandomNode>& node, int curr_depth) const {
	if (node->has_target())
		return curr_depth + 1;

	return std::max(get_depth(node->left, curr_depth + 1), get_depth(node->right, curr_depth + 1));
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

std::unique_ptr<RandomTree> RandomTree::node_to_tree(std::unique_ptr<RandomNode> node) {
	auto tree = std::make_unique<RandomTree>(0, 0);
	tree->root = std::move(node);

	return tree;
}

void RandomTree::switch_nodes(RandomNode* node1, RandomNode* node2, bool go_left1, bool go_left2) const {
	if (node1->has_target() || node2->has_target()) {
		auto tmp = node1->copy();
		node1->copy_from(node2->copy()); // any better way to convert node2 from pointer to unique ptr?
		node2->copy_from(tmp);
	} else {
		auto& tmp_ptr_1 = go_left1 ? node1->left : node1->right;
		auto tmp_ptr1_copy = tmp_ptr_1->copy();
		auto& tmp_ptr_2 = go_left2 ? node2->left : node2->right;

		tmp_ptr_1 = std::move(tmp_ptr_2);
		tmp_ptr_2 = std::move(tmp_ptr1_copy);
	}
}
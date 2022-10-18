// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomTree.hpp"
#include <random>
#include <DataFrame/DataFrame.h>
#include <fstream>

std::vector<std::vector<int>> load_dataset(std::string dataset_path_name);
std::tuple<std::vector<std::vector<int>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<int>> split_data(const std::vector<std::vector<int>>& data, double split_percentage);
void sort_population(std::vector<std::pair<std::unique_ptr<RandomTree>,double>>& population);
void insert_sorted(std::vector<std::pair<std::unique_ptr<RandomTree>,double>>& vec, std::unique_ptr<RandomTree> item, double item_acc);
std::pair<double, std::unique_ptr<RandomTree>> run(int population_size, double left_prob, double right_prob, int max_nodes, int max_depth,
	double rec_prob, double mut_prob, const std::vector<std::vector<int>>& X, const std::vector<int> y, int num_children, int patience = 500);
std::tuple<double, double, std::unique_ptr<RandomTree>> run_with_executions(int population_size, double left_prob, double right_prob, int max_nodes, int max_depth,
	double rec_prob, double mut_prob, const std::vector<std::vector<int>>& X, const std::vector<int> y, int num_children, int executions = 30, int patience = 500);

int main() {
	using namespace std;

	const int NUM_EXEC = 30;
	const double TRAINING_PERCENTAGE = 0.70;

	vector<int> population_sizes = { 20, 50, 100, 250 };
	vector<double> left_right_prob = { 0.5, 0.6, 0.7 };
	vector<int> max_nodes = { 30, 40, 50 };
	vector<int> max_depth = { 5, 10, 15 };
	vector<double> rec_prob = { 0.1, 0.2, 0.3 };
	vector<double> mut_prob = { 0.1, 0.2, 0.3 };
	vector<int> children_creation = { 2, 5, 10, 50, 100 };

	auto data = load_dataset("/dataset/Dermatology_new_features_selected.csv");

	// shuffle
	auto rng = std::default_random_engine{};
	std::shuffle(data.begin(), data.end(), rng);

	auto [X_train, y_train, X_test, y_test] = split_data(data, TRAINING_PERCENTAGE);

	double best_acc_pop = 0;
	int best_pop = population_sizes[0];
	for (auto pop_size : population_sizes) {
		auto [acc, _, __] = run_with_executions(pop_size, left_right_prob[0], left_right_prob[0], max_nodes[0], max_depth[0], rec_prob[0], mut_prob[0], X_train, y_train, children_creation[0]);
		std::cout << "Acc for population of " << pop_size << ": " << acc << std::endl;
		if (acc > best_acc_pop) {
			best_pop = pop_size;
			best_acc_pop = acc;
		}
	}
	std::cout << "Best acc is with Population Size: " << best_pop << std::endl;

	double best_acc_lr_prob = 0;
	int best_lr_prob = left_right_prob[0];
	for (auto lr_prob : left_right_prob) {
		auto [acc, _, __] = run_with_executions(best_pop, lr_prob, left_right_prob[0], max_nodes[0], max_depth[0], rec_prob[0], mut_prob[0], X_train, y_train, children_creation[0]);
		std::cout << "Acc for left and right prob of " << lr_prob << ": " << acc << std::endl;
		if (acc > best_acc_lr_prob) {
			best_lr_prob = lr_prob;
			best_acc_lr_prob = acc;
		}
	}
	std::cout << "Best acc is with left and right prob of: " << best_lr_prob << std::endl;

	double best_acc_max_node = 0;
	int best_max_node = max_nodes[0];
	for (auto max_n : max_nodes) {
		auto [acc, _, __] = run_with_executions(best_pop, best_lr_prob, best_lr_prob, max_n, max_depth[0], rec_prob[0], mut_prob[0], X_train, y_train, children_creation[0]);
		std::cout << "Acc for max nodes of " << max_n << ": " << acc << std::endl;
		if (acc > best_acc_max_node) {
			best_max_node = max_n;
			best_acc_max_node = acc;
		}
	}
	std::cout << "Best acc is with max nodes of: " << best_max_node << std::endl;

	double best_acc_max_depth = 0;
	int best_max_depth = max_depth[0];
	for (auto m_depth : max_depth) {
		auto [acc, _, __] = run_with_executions(best_pop, best_lr_prob, best_lr_prob, best_max_node, m_depth, rec_prob[0], mut_prob[0], X_train, y_train, children_creation[0]);
		std::cout << "Acc for max depth of " << m_depth << ": " << acc << std::endl;
		if (acc > best_acc_max_depth) {
			best_max_depth = m_depth;
			best_acc_max_depth = acc;
		}
	}
	std::cout << "Best acc is with max depth of: " << best_max_depth << std::endl;

	double best_acc_rec_prob = 0;
	int best_rec_prob = rec_prob[0];
	for (auto r_prob : rec_prob) {
		auto [acc, _, __] = run_with_executions(best_pop, best_lr_prob, best_lr_prob, best_max_node, best_max_depth, r_prob, mut_prob[0], X_train, y_train, children_creation[0]);
		std::cout << "Acc for rec prob of " << r_prob << ": " << acc << std::endl;
		if (acc > best_acc_rec_prob) {
			best_rec_prob = r_prob;
			best_acc_rec_prob = acc;
		}
	}
	std::cout << "Best acc is with rec prob of: " << best_rec_prob << std::endl;

	double best_acc_mut_prob = 0;
	int best_mut_prob = mut_prob[0];
	for (auto m_prob : mut_prob) {
		auto [acc, _, __] = run_with_executions(best_pop, best_lr_prob, best_lr_prob, best_max_node, best_max_depth, best_rec_prob, m_prob, X_train, y_train, children_creation[0]);
		std::cout << "Acc for mut prob of " << m_prob << ": " << acc << std::endl;
		if (acc > best_acc_mut_prob) {
			best_mut_prob = m_prob;
			best_acc_mut_prob = acc;
		}
	}
	std::cout << "Best acc is with mut prob of: " << best_mut_prob << std::endl;

	unique_ptr<RandomTree> best_tree = make_unique<RandomTree>(0, 0, 1, 1);
	double best_acc_children = 0;
	int best_num_children = children_creation[0];
	double best_stddev = 0;
	for (auto children : children_creation) {
		auto [acc, stddev, tree] = run_with_executions(best_pop, best_lr_prob, best_lr_prob, best_max_node, best_max_depth, best_rec_prob, best_mut_prob, X_train, y_train, children);
		std::cout << "Acc for number of children " << children << ": " << acc << std::endl;
		if (acc > best_acc_children) {
			best_num_children = children;
			best_acc_children = acc;
			best_tree = std::move(tree);
			best_stddev = stddev;
		}
	}
	std::cout << "Best number of children creation is with: " << best_num_children << std::endl;

	cout << "Best result acc: " << best_acc_children << " (stddev: " << best_stddev << ")" << endl;
	best_tree->print(string(ROOT_DIR) + "Best_Tree.txt");

	ofstream best_tree_out_file(string(ROOT_DIR) + "Best_Tree.txt", ios_base::app);
	best_tree_out_file << "\n\n\nAcc: " << best_acc_children << " (stddev: " << best_stddev << ")" << endl;
	best_tree_out_file.close();
}

std::tuple<double, double, std::unique_ptr<RandomTree>> run_with_executions(int population_size, double left_prob, double right_prob, int max_nodes, int max_depth,
	double rec_prob, double mut_prob, const std::vector<std::vector<int>>& X, const std::vector<int> y, int num_children, int executions, int patience) {
	using namespace std;

	vector<double> accs = {};
	double best_acc = 0;
	auto best_tree = make_unique<RandomTree>(0, 0, 1, 1);
	for (int i = 0; i < executions; i++) {
		auto [acc, tree] = run(population_size, left_prob, right_prob, max_nodes, max_depth, rec_prob, mut_prob, X, y, num_children, patience);
		if (acc > best_acc) {
			best_acc = acc;
			best_tree = std::move(tree);
		}
		accs.push_back(acc);
	}

	double avg = Utils::get_average(accs);

	return make_tuple(avg, Utils::get_stddev(accs, avg), std::move(best_tree));
}

std::pair<double, std::unique_ptr<RandomTree>> run(int population_size, double left_prob, double right_prob, int max_nodes, int max_depth,
	double rec_prob, double mut_prob, const std::vector<std::vector<int>>& X, const std::vector<int> y, int num_children, int patience) {

	using namespace std;

	vector<pair<unique_ptr<RandomTree>, double>> population;

	for (int i = 0; i < population_size; i++) {
		auto tree = make_unique<RandomTree>(left_prob, right_prob, max_nodes, max_depth);
		population.push_back(make_pair(move(tree), tree->get_accuracy(X, y)));
	}

	sort_population(population);

	double best_acc = population[0].second;
	
	unique_ptr<RandomTree> best_tree = make_unique<RandomTree>(0,0,1,1);

	int curr_patience = 0;

	while (curr_patience < patience) {
		auto pai1 = population[0].first->copy();
		auto pai2 = population[1].first->copy();

		for (int i = 0; i < num_children / 2; i++) {
			auto [filho1, filho2] = pai1->recombine(pai2, rec_prob);
			filho1->mutate_comparating_value(mut_prob);
			filho1->mutate_comparator(mut_prob);
			filho1->mutate_target();
			filho2->mutate_comparating_value(mut_prob);
			filho2->mutate_comparator(mut_prob);
			filho2->mutate_target();

			insert_sorted(population, std::move(filho1), filho1->get_accuracy(X, y));
			insert_sorted(population, std::move(filho2), filho2->get_accuracy(X, y));
		}

		for (int i = 0; i < num_children / 2; i++) {
			population.pop_back();
			population.pop_back();
		}

		auto curr_best_acc = population[0].second;

		curr_patience++;

		if (curr_best_acc > best_acc) {
			best_acc = curr_best_acc;
			best_tree = population[0].first->copy();
			curr_patience = 0;
		}
	}
	
	return make_pair(best_acc, std::move(best_tree));
}

void sort_population(std::vector<std::pair<std::unique_ptr<RandomTree>, double>>& population) {
	using namespace std;

	auto sort_fun = [](const std::pair<std::unique_ptr<RandomTree>, double>& rt1, const std::pair<std::unique_ptr<RandomTree>, double>& rt2) {
		return rt1.second > rt2.second; // desc order
	};

	sort(population.begin(), population.end(), sort_fun);
}

void insert_sorted(std::vector<std::pair<std::unique_ptr<RandomTree>,double>>& vec, std::unique_ptr<RandomTree> item, double item_acc) {
	for (int i = 0; i < vec.size(); i++) {
		if (item_acc > vec[i].second) {
			vec.insert(vec.begin() + i, std::make_pair(std::move(item), item_acc));
			return;
		}
	}

	vec.push_back(std::make_pair(std::move(item), item_acc));
}

std::vector<std::vector<int>> load_dataset(std::string dataset_path_name) {
	using namespace std;
	using namespace hmdf;
	using StrDataFrame = StdDataFrame<int>;
	StrDataFrame ibm_df;

	ibm_df.read((std::string(ROOT_DIR) + dataset_path_name).c_str(), io_format::csv2);

	auto [num_rows, num_cols] = ibm_df.shape();

	vector<vector<int>> data_as_vector;

	for (int i = 0; i < num_rows; i++) {
		auto row = ibm_df.get_row<int>(i);
		auto& Xi = row.get_vector<int>();
		data_as_vector.push_back(Xi);
	}

	return data_as_vector;
}

std::tuple<std::vector<std::vector<int>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<int>> split_data(const std::vector<std::vector<int>>& data, double split_percentage) {
	using namespace std;

	vector<vector<int>> X;
	vector<int> y;

	for (auto elem : data) {
		y.push_back(elem.back());

		elem.pop_back(); // removing 'y'
		X.push_back(elem);
	}

	int training_last_row = (int) (data.size() * split_percentage);

	vector<vector<int>> X_train;
	vector<int> y_train;
	vector<vector<int>> X_test;
	vector<int> y_test;

	for (int i = 0; i <= training_last_row; i++) {
		X_train.push_back(X[i]);
		y_train.push_back(y[i]);
	}

	for (int i = training_last_row + 1; i < X.size(); i++) {
		X_test.push_back(X[i]);
		y_test.push_back(y[i]);
	}

	return std::make_tuple(X_train, y_train, X_test, y_test);
}
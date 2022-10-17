// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomTree.hpp"
#include <random>
#include <DataFrame/DataFrame.h>


std::vector<std::vector<int>> load_dataset(std::string dataset_path_name);
std::tuple<std::vector<std::vector<int>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<int>> split_data(const std::vector<std::vector<int>>& data, double split_percentage);
void sort_population(std::vector<std::unique_ptr<RandomTree>>& population, const std::vector<std::vector<int>>& X, const std::vector<int>& y);
void insert_sorted(std::vector<std::unique_ptr<RandomTree>>& vec, std::unique_ptr<RandomTree> item, const std::vector<std::vector<int>>& X, const std::vector<int>& y);

int main() {
	using namespace std;

	const int POPULATION_SIZE = 100;
	const double LEFT_PROB = 0.6;
	const double RIGHT_PROB = 0.6;
	const int MAX_NODES = 50;
	const int MAX_DEPTH = 15;

	const int PATIENCE = 50;

	const double TRAINING_PERCENTAGE = 0.70;


	auto data = load_dataset("/dataset/Dermatology_new.csv");	

	// shuffle
	auto rng = std::default_random_engine{};
	std::shuffle(data.begin(), data.end(), rng);

	auto [X_train, y_train, X_test, y_test] = split_data(data, TRAINING_PERCENTAGE);

	vector<unique_ptr<RandomTree>> population;

	for (int i = 0; i < POPULATION_SIZE; i++)
		population.push_back(make_unique<RandomTree>(LEFT_PROB, RIGHT_PROB, MAX_NODES, MAX_DEPTH));

	sort_population(population, X_train, y_train);

	double best_acc = population[0]->get_accuracy(X_train, y_train);

	int curr_patience = 0;

	while (curr_patience < PATIENCE) {
		auto& pai1 = population[0];
		auto& pai2 = population[1];
		auto [filho1, filho2] = pai1->recombine(pai2, 0.1);
		filho1->mutate_comparating_value(0.2);
		filho1->mutate_comparator(0.2);
		filho1->mutate_target();
		filho2->mutate_comparating_value(0.2);
		filho2->mutate_comparator(0.2);
		filho2->mutate_target();

		insert_sorted(population, std::move(filho1), X_train, y_train);
		insert_sorted(population, std::move(filho2), X_train, y_train);
		population.pop_back();
		population.pop_back();
		
		auto curr_best_acc = population[0]->get_accuracy(X_train, y_train);

		if (curr_best_acc > best_acc) {
			std::cout << "Acc improved from " << best_acc << " to " << curr_best_acc << std::endl;
			best_acc = curr_best_acc;
		}
	}

	cin.get();
}

void sort_population(std::vector<std::unique_ptr<RandomTree>>& population, const std::vector<std::vector<int>>& X, const std::vector<int>& y) {
	using namespace std;

	auto sort_fun = [X, y](const unique_ptr<RandomTree>& rt1, const unique_ptr<RandomTree>& rt2) {
		return rt1->get_accuracy(X, y) > rt2->get_accuracy(X, y); // desc order
	};

	sort(population.begin(), population.end(), sort_fun);
}

void insert_sorted(std::vector<std::unique_ptr<RandomTree>>& vec, std::unique_ptr<RandomTree> item, const std::vector<std::vector<int>>& X, const std::vector<int>& y)
{
	auto acc = item->get_accuracy(X, y);

	for (int i = 0; i < vec.size(); i++) {
		if (acc > vec[i]->get_accuracy(X, y)) {
			vec.insert(vec.begin() + i, std::move(item));
			return;
		}
	}

	vec.push_back(std::move(item));
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
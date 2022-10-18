#pragma once
#include <vector>
#include <random>
#include <numeric>

class Utils {
public:
	template <typename T>
	static T get_random_elem(const std::vector<T>& in, std::mt19937& gen) {
		std::uniform_int_distribution<> distr(0, (int)in.size() - 1);
		return in.at(distr(gen));
	}

	static std::vector<int> make_vector_from_i_to_n(int i, int n);
	static std::vector<int> make_vector_from_i_to_n(int i, int n, int steps);
	
	template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	constexpr static double get_average(const std::vector<T>& in) {
		if (in.empty()) return 0;

		const double count = (double) in.size();
		return std::reduce(in.begin(), in.end()) / count;
	}
	template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	constexpr static double get_stddev(const std::vector<T>& in, double avg) {
		double sum_pow_diff_mean = 0;
		for (T elem : in)
			sum_pow_diff_mean += pow(elem - avg, 2);

		return sqrt(sum_pow_diff_mean / in.size());
	}
	template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	constexpr static double get_stddev(const std::vector<T>& in) {
		auto avg = get_average(in);
		return get_stddev(in, avg);
	}
};

class NodeSeed {
public:
	static NodeSeed& getInstance()
	{
		static NodeSeed instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

	NodeSeed(NodeSeed const&) = delete;
	void operator=(NodeSeed const&) = delete;

	int get_seed() { return seed++; }
private:
	NodeSeed() {}
	int seed = 27'03;
};

class TreeSeed {
public:
	static TreeSeed& getInstance()
	{
		static TreeSeed instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

	TreeSeed(TreeSeed const&) = delete;
	void operator=(TreeSeed const&) = delete;

	int get_seed() { return seed++; }
private:
	TreeSeed() {}
	int seed = 0327;
};
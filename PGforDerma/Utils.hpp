#pragma once
#include <vector>
#include <random>

class Utils {
public:
	template <typename T>
	static T get_random_elem(const std::vector<T>& in, std::mt19937& gen) {
		std::uniform_int_distribution<> distr(0, (int)in.size() - 1);
		return in.at(distr(gen));
	}

	static std::vector<int> make_vector_from_i_to_n(int i, int n);
	static std::vector<int> make_vector_from_i_to_n(int i, int n, int steps);
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
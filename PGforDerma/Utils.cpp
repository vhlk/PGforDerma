#include "Utils.hpp"


std::vector<int> Utils::make_vector_from_i_to_n(int i, int n) {
	std::vector<int> res;

	for (int j = i; j <= n; j++) res.push_back(j);

	return res;
}

std::vector<int> Utils::make_vector_from_i_to_n(int i, int n, int steps) {
	std::vector<int> res;

	for (int j = i; j <= n; j += steps) res.push_back(j);

	return res;
}
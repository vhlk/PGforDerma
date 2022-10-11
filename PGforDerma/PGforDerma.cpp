// PGforDerma.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include "PGforDerma.hpp"
#include "Definitions.hpp"
#include "RandomNode.hpp"
#include "RandomTree.hpp"
#include <array>
#include <random>
#include "csv.h"

int main()
{
	Conversions conversions;

	io::CSVReader<35> in(std::string(ROOT_DIR) + "/dataset/Dermatology.csv");
	in.read_header(io::ignore_no_column, conversions.get_feature_name(1), conversions.get_feature_name(2), conversions.get_feature_name(3), conversions.get_feature_name(4), conversions.get_feature_name(5), conversions.get_feature_name(6), conversions.get_feature_name(7), conversions.get_feature_name(8), conversions.get_feature_name(9), conversions.get_feature_name(10), conversions.get_feature_name(11), conversions.get_feature_name(12), conversions.get_feature_name(13), conversions.get_feature_name(14), conversions.get_feature_name(15), conversions.get_feature_name(16), conversions.get_feature_name(17), conversions.get_feature_name(18), conversions.get_feature_name(19), conversions.get_feature_name(20), conversions.get_feature_name(21), conversions.get_feature_name(22), conversions.get_feature_name(23), conversions.get_feature_name(24), conversions.get_feature_name(25), conversions.get_feature_name(26), conversions.get_feature_name(27), conversions.get_feature_name(28), conversions.get_feature_name(29), conversions.get_feature_name(30), conversions.get_feature_name(31), conversions.get_feature_name(32), conversions.get_feature_name(33), conversions.get_feature_name(34), "Dissease");

	int f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, f32, f33, f34, dissease;
	in.read_row(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, f32, f33, f34, dissease);

	std::cout << f1 << std::endl;
	std::cout << f34 << std::endl;
	std::cout << dissease << std::endl;

	auto tree = std::make_unique<RandomTree>(0.4, 0.4);

	std::cout << "printing tree...\n" << std::endl;
	
	tree->print();
}
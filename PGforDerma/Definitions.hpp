#pragma once
#include <iostream>
#include <array>
#include "Utils.hpp"

struct PossibleValues {
	const std::vector<int> comparators = Utils::make_vector_from_i_to_n(1, 6);
	const std::vector<int> comparators_age = { 1, 2, 3, 4 };
	const std::vector<int> features = {5, 6, 9, 11, 13, 15, 19, 20, 22, 23, 24, 26, 29};
	const std::vector<int> values = Utils::make_vector_from_i_to_n(0, 3);
	const std::vector<int> values_hist_familiar = Utils::make_vector_from_i_to_n(0, 1);
	const std::vector<int> values_age = Utils::make_vector_from_i_to_n(15, 60, 15);
	const std::vector<int> targets = Utils::make_vector_from_i_to_n(1, 6);
};

struct Conversions {
	constexpr int get_index_for_feature(int feature) {
		PossibleValues pv;
		for (int i = 0; i < pv.features.size(); i++) {
			if (pv.features[i] == feature)
				return i;
		}

		throw std::invalid_argument("index out of range");
	}
	constexpr std::string get_string_comparator(int comparator) {
		switch (comparator) {
			case (1): return ">";
			case (2): return "<";
			case (3): return ">=";
			case (4): return "<=";
			case (5): return "!=";
			case (6): return "==";
			default:
				throw std::invalid_argument("index out of range");
		}
	}

	constexpr std::string get_feature_name(int feature) {
		switch (feature) {
			case (1): return "Erythema";
			case (2): return "Scaling";
			case (3): return "Definite Borders";
			case (4): return "Itching";
			case (5): return "Koebner Phenomenon";
			case (6): return "Polygonal Papules";
			case (7): return "Follicular Papules";
			case (8): return "Oral Mucosal Involvement";
			case (9): return "Knee and Elbow Involvement";
			case (10): return "Scalp Involvement";
			case (11): return "Family History";
			case (12): return "Melanin Incontinence";
			case (13): return "Eosinophils in the Infiltrate";
			case (14): return "PNL Infiltrate";
			case (15): return "Fibrosis of the Papillary Dermis";
			case (16): return "Exocytosis";
			case (17): return "Acanthosis";
			case (18): return "Hyperkeratosis";
			case (19): return "Parakeratosis";
			case (20): return "Clubbing of the Rete Ridges";
			case (21): return "Elongation of the Rete Ridges";
			case (22): return "Thinning of the Suprapapillary Epidermis";
			case (23): return "Spongiform Pustule";
			case (24): return "Munro Microabcess";
			case (25): return "Focal Hypergranulosis";
			case (26): return "Disappearance of the Granular Layer";
			case (27): return "Vacuolisation and Damage of Basal Layer";
			case (28): return "Spongiosis";
			case (29): return "Saw-tooth Appearance of Retes";
			case (30): return "Follicular Horn Plug";
			case (31): return "Perifollicular Parakeratosis";
			case (32): return "Inflammatory Monoluclear Inflitrate";
			case (33): return "Band-like Infiltrate";
			case (34): return "Age";
			default:
				throw std::invalid_argument("index out of range");
		}
	}

	constexpr std::string get_target_name(int feature) {
		switch (feature) {
			case (1): return "psoriasis";
			case (2): return "seboreic dermatitis";
			case (3): return "lichen planus";
			case (4): return "pityriasis rosea";
			case (5): return "cronic dermatitis";
			case (6): return "pityriasis rubra pilaris";
			default:
				throw std::invalid_argument("index out of range");
		}
	}
};

struct Comparations {
	constexpr bool compare(int feature_value, int comparator, int comparing_value) {
		Conversions conversions;

		return compare(feature_value, conversions.get_string_comparator(comparator), comparing_value);
	}

	constexpr bool compare(int feature_value, std::string comparator, int comparing_value) {
		if (comparator == ">") {
			return feature_value > comparing_value;
		} else if (comparator == "<") {
			return feature_value < comparing_value;
		} else if (comparator == ">=") {
			return feature_value >= comparing_value;
		} else if (comparator == "<=") {
			return feature_value <= comparing_value;
		} else if (comparator == "!=") {
			return feature_value != comparing_value;
		} else if (comparator == "==") {
			return feature_value == comparing_value;
		} else {
			throw std::invalid_argument("Comparator '" + comparator + "' not found");
		}
	}
};
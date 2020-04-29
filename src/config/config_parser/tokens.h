#pragma once

#include <string>

enum tokens {
	identifiers,  //Basically name of a variable
	//arithmetic_operators,
	//rational_operators,
	//logical_operators,
	//unary_operators,
	assignment_operators,
	//literals,     //ex. 2, true or "hello"
	string_literal,
	path_literal,
	//keywords,     //if -- else  
	separators,   // {} ; 
	comment,      //this
	type
};

struct types {
	static const int size_of_array = 6;
	static const std::string s_types[size_of_array];

	static bool check_type(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s == s_types[a])
				return true;
		return false;
	}

};

//struct arithmetic_operators {
//	static const int size_of_array = 5;
//	static const std::string s_arithmetic_operators[size_of_array];
//
//	static bool check_type(const std::string s) {
//		for (int a = 0; a < size_of_array; a++)
//			if (s == s_arithmetic_operators[a])
//				return true;
//		return false;
//	}
//
//};
//
//struct rational_operators {
//	static const int size_of_array = 6;
//	static const std::string s_rational_operators[size_of_array];
//
//	static bool check_type(const std::string s) {
//		for (int a = 0; a < size_of_array; a++)
//			if (s == s_rational_operators[a])
//				return true;
//		return false;
//	}
//
//};
//
//struct logical_operators {
//	static const int size_of_array = 2;
//	static const std::string s_logical_operators[size_of_array];
//
//	static bool check_type(const std::string s) {
//		for (int a = 0; a < size_of_array; a++)
//			if (s == s_logical_operators[a])
//				return true;
//		return false;
//	}
//
//};
//
//struct unary_operators {
//	static const int size_of_array = 5;
//	static const std::string s_unary_operators[size_of_array];
//
//	static bool check_type(const std::string s) {
//		for (int a = 0; a < size_of_array; a++)
//			if (s == s_unary_operators[a])
//				return true;
//		return false;
//	}
//};
//
struct assignment_operators {
	static const int size_of_array = 2;
	static const std::string s_assignment_operators[size_of_array];

	static bool check_type(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s == s_assignment_operators[a])
				return true;
		return false;
	}
};

struct separators {
	static const int size_of_array = 8;
	static const std::string s_separators[size_of_array];

	static bool check_type(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s == s_separators[a])
				return true;
		return false;
	}

	static bool check_type(const char c) {
		for (int a = 0; a < size_of_array; a++) {
			if (c == s_separators[a][0])
				return true;
		}
		return false;
	}

	static int check_type_ret_position(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s.find(s_separators[a]) != std::string::npos)
				return s.find_first_of(s_separators[a]);
		return -1;
	}

};

struct predefined_literals {
	static const int size_of_array = 5;
	static const std::string s_predefined_literals[size_of_array];

	static bool check_type(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s == s_predefined_literals[a])
				return true;
		return false;
	}
};

struct keywords {
	static const int size_of_array = 7;
	static const std::string s_keywords[size_of_array];

	static bool check_type(const std::string s) {
		for (int a = 0; a < size_of_array; a++)
			if (s == s_keywords[a])
				return true;
		return false;
	}
};

struct tokenizer_data {
	tokens type;
	std::string value;
	int index;
};
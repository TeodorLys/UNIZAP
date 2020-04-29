#include "error_handler.h"
#include <iostream>

void error_handler::call_error_and_exit(std::string s) {
	printf("%s, exiting...\n", s.c_str());

	std::cin.get();

	exit(0);
}
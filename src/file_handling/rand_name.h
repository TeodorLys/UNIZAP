#pragma once
#include <string>
#include <random>
#include <time.h>
class rand_name {
private:
	const std::string available = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
	std::string buffer;
public:
	rand_name(int size, int seed = time(NULL)) {
		srand(seed);
		if (size < 3)
			size = 3;

		for (int a = 0; a < size; a++) {
			int i = rand() % available.size();
			buffer += available[i];
		}
	}

	std::string get_name() const { return buffer; }

};

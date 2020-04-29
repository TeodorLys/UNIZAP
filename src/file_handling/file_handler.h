#pragma once
#include <filesystem>
#include <string>
class file_handler {

public:
	/*
	Gets the size of an entire folder
	*/
	static long long get_dir_size(std::string path) {
		long long size = 0;
		for (std::filesystem::recursive_directory_iterator it(path); it != std::filesystem::recursive_directory_iterator(); it++) {
			if (!std::filesystem::is_directory(*it)) {
				size += std::filesystem::file_size(*it);
			}
		}
		return size / 1000;
	}

};


#pragma once
#include <string>
#include <zip.h>
#include <filesystem>

class zip_handler {
private:
	zip_source_t* zs;
	zip_t* z;
	int error = 0;
	std::string zip_path;

public:

	/*
	*Path - Full output path
	TODO: Make this a boolean and have more error checking...
	*/
	void create_zip(std::filesystem::path path);
	void open_zip(std::filesystem::path path);
	/*
	TODO: Make this a boolean and have more error checking...
	*/
	void add_file_to_zip(std::filesystem::path path, std::string dir = "",bool print_name = false);
	void add_dir_to_zip(std::string name);
	void unzip(std::filesystem::path output);
	bool delete_zip();

	void close_zip() {
		zip_close(z);
	}

};


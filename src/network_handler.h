#pragma once
#include <curl/curl.h>
#include <string>
#include <filesystem>

class network_handler {
public:
	bool upload_to_dropbox(std::filesystem::path path, std::string access);
	bool download_from_dropbox(std::filesystem::path path, std::string output, std::string access);
};


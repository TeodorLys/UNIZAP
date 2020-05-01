#pragma once
#include <filesystem>
#include <string>
#include "../user_communication/error_handler.h"

class format_file_size {
private:
	long long file_size = 0;
	long kilo = 0;
	long mega = 0;
	long giga = 0;
	long tera = 0;
	long peta = 0;
public:
	enum metric {
		BYTE,
		KILO,
		MEGA,
		GIGA,
		TERA,
		PETA
	};

	format_file_size(std::filesystem::path path) {
		if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
			return;
			//error_handler::call_error_and_exit("[FORMAT_FILE_SIZE] Path does not exists...");
		file_size = std::filesystem::file_size(path);
		kilo = (long)(file_size / 1000);
		mega = (long)(file_size / 1000000);
		giga = (long)(file_size / 1000000000);
		//These two probably wont be used that much
		tera = (long)(file_size / 1000000000000);
		peta = (long)(file_size / 1000000000000000);
	}
	/*
	Returns the size in given format, see enum above
	*/
	long long size(metric m) {
		switch (m)
		{
		case BYTE:
			return file_size; 
			break;
		case KILO:
			return kilo;
			break;
		case MEGA:
			return mega;
			break;
		case GIGA:
			return giga;
			break;
		case TERA:
			return tera;
			break;
		case PETA:
			return peta;
			break;
		default:
			return 0;
			break;
		}
	}

	std::string formatted_size() {
		if (peta > 0)
			return std::string(std::to_string(peta) + "PB");
		else if(tera > 0)
			return std::string(std::to_string(tera) + "TB");
		else if(giga > 0)
			return std::string(std::to_string(giga) + "GB");
		else if(mega > 0)
			return std::string(std::to_string(mega) + "MB");
		else if(kilo > 0)
			return std::string(std::to_string(kilo) + "kB");
		else
			return std::string(std::to_string(file_size) + "b");
	}

};
#include <fstream>
#include "zip_handler.h"
#include "../user_communication/error_handler.h"

void zip_handler::create_zip(std::filesystem::path path) {
	if (std::filesystem::exists(path))
		std::filesystem::rename(path, path.string() + ".old");
	z = zip_open(path.string().c_str(), ZIP_CREATE, &error);
	if (error)
		error_handler::call_error_and_exit(zip_strerror(z));
}


void zip_handler::open_zip(std::filesystem::path path) {
	z = zip_open(path.string().c_str(), ZIP_RDONLY, &error);
	if (error)
		error_handler::call_error_and_exit(zip_strerror(z));
}

void zip_handler::unzip(std::filesystem::path output) {
	char buff[100];
	struct zip_stat st;
	zip_file* f;
	for (int a = 0; a < zip_get_num_entries(z, 0); a++) {
		if (zip_stat_index(z, a, 0, &st) == 0) {
			f = zip_fopen_index(z, a, 0);
			std::string file_name = output.string() + "/";
			file_name += st.name;
			file_name.replace(file_name.begin(), file_name.end(), '\\', '/');
			if (file_name[file_name.size() - 1] == '/') {
				std::filesystem::create_directory(file_name);
				continue;
			}

			std::fstream file(file_name, std::fstream::binary | std::fstream::out);

			if (!file.is_open())
				error_handler::call_error_and_exit("[ZIP_ERROR] Could not open file " + file_name);

			int sum = 0;
			int len = 0;
			while (sum != st.size) {
				len = (int)zip_fread(f, buff, 100);
				file.write(buff, len);
				sum++;
			}
			file.close();
			zip_fclose(f);
		}
	}
	close_zip();
}

void zip_handler::add_file_to_zip(std::filesystem::path path, std::string dir, bool print_name) {
	zs = zip_source_file(z, path.string().c_str(), 0, 0);

	if (zs == NULL)
		error_handler::call_error_and_exit("[ZIP_ERROR] Could not enumerate file, zip_source_file, " + std::string(zip_strerror(z)));

	if (dir != "") {
		dir += "/" + path.filename().string();
		int e = (int)zip_file_add(z, dir.c_str(), zs, ZIP_FL_OVERWRITE);
		if (e < 0)
			error_handler::call_error_and_exit("[ZIP_ERROR] Could not add file, " + std::string(zip_strerror(z)));
	}
	else {
		int e = (int)zip_file_add(z, path.filename().string().c_str(), zs, ZIP_FL_OVERWRITE);
		if (e < 0)
			error_handler::call_error_and_exit("[ZIP_ERROR] Could not add file, " + std::string(zip_strerror(z)));
	}
	
	if (print_name)
		printf("%s\n", path.filename().string().c_str());
}

void zip_handler::add_dir_to_zip(std::string name) {
	int e = (int)zip_dir_add(z, name.c_str(), ZIP_FL_ENC_UTF_8);
	if (e < 0)
		error_handler::call_error_and_exit("[ZIP_ERROR] Could not add directory, " + std::string(zip_strerror(z)));
}

bool zip_handler::delete_zip() {
	try {
		std::filesystem::remove(zip_path);
	}
	catch (std::filesystem::filesystem_error e) {
		return false;
	}
	return true;
}

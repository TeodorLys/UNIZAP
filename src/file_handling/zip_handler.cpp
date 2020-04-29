#include "zip_handler.h"
#include "error_handler.h"
void zip_handler::create_zip(std::filesystem::path path) {
	z = zip_open(path.string().c_str(), ZIP_CREATE, &error);
	if (error)
		error_handler::call_error_and_exit(zip_strerror(z));
}

void zip_handler::add_file_to_zip(std::filesystem::path path, std::string dir,bool print_name) {
	zs = zip_source_file(z, path.string().c_str(), 0, 0);

	if (dir != "") {
		dir += "/" + path.filename().string();
		zip_file_add(z, dir.c_str(), zs, ZIP_FL_OVERWRITE);
	}
	else {
		zip_file_add(z, path.filename().string().c_str(), zs, ZIP_FL_OVERWRITE);
	}
	
	if (print_name)
		printf("%s\n", path.filename().string().c_str());
}

void zip_handler::add_dir_to_zip(std::string name) {
	zip_dir_add(z, name.c_str(), ZIP_FL_ENC_UTF_8);
}

bool zip_handler::delete_zip() {
	try {
		std::filesystem::remove(zip_path);
	}
	catch (std::filesystem::filesystem_error& e) {
		return false;
	}
	return true;
}

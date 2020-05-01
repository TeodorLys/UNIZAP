#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <algorithm>
#include "config/config_parser/parser.h"
#include "user_communication/error_handler.h"
#include "file_handling/zip_handler.h"
#include "network_handler.h"
#include "visual_goodness/progress.h"
#include "file_handling/rand_name.h"
#include "file_handling/format_file_size.h"

struct file_data {
	std::string name;
	std::string full_path;

	file_data(std::string _name, std::string _full_path) : name(_name), full_path(_full_path){}
};

std::vector<std::string> files;
std::vector<file_data> all_files;
parser _parser;
zip_handler zip;



void zip_single_files() {
	for (int a = 0; a < (int)files.size(); a++) {
		zip.add_file_to_zip(files[a]);
		all_files.push_back(file_data(files[a], files[a]));
	}
}


void zip_prepare(std::string zip_name) {
	printf("Creating zip file...");
	zip.create_zip(zip_name);
	printf("Done\n");
}

/*
This function needs to be looked at!
This is probably the janqiest function I have ever made...
TODO: Add some error checks, try{ etc...
*/
void zip_up_directory(std::string path, std::string zip_name, bool include_dir, bool add_scoped_files) {

	zip_prepare(zip_name);

	std::string base_path = std::filesystem::absolute(path).string();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (include_dir) {
		std::string b_dir_name;
		b_dir_name = base_path;
#ifdef __linux__
		b_dir_name.erase(0, b_dir_name.rfind("/") + 1);

		zip.add_dir_to_zip(b_dir_name);

		base_path.erase(base_path.rfind("/"), base_path.length());
#else
		b_dir_name.erase(0, b_dir_name.rfind("\\") + 1);

		zip.add_dir_to_zip(b_dir_name);

		base_path.erase(base_path.rfind("\\"), base_path.length());
#endif
	}

	std::vector<std::filesystem::path> _p;

	for (std::filesystem::recursive_directory_iterator it(path); it != std::filesystem::recursive_directory_iterator(); it++) {
		_p.push_back(std::filesystem::absolute((*it).path()));
	}

	int file_count = _p.size() + files.size();
	progress __p("Adding files: ", 40, file_count);

	/*
	here we just create the directory entries in the zip file, and also removing them
	from the vector of files. see above
	*/
	for (int a = 0; a < (int)_p.size(); a++) {
		std::string _d = _p[a].string();
		std::string buffer = _d;
		_d.erase(0, base_path.length() + 1);
		all_files.push_back(file_data(_d, buffer));
		if (std::filesystem::is_directory(buffer)) {
			//_d.erase(0, base_path.length() + 1);
			__p.one_forward();
			zip.add_dir_to_zip(_d);
			_p.erase(_p.begin() + a);
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	/*
	Atlast we add the files, we need to parse the directory, to put the files into the 
	correct folder(in the zip file)
	*/
	for (int a = 0; a < (int)_p.size(); a++) {
		__p.one_forward();
		std::string _d = _p[a].string();
		std::string buff = _d;
		_d.erase(0, base_path.length() + 1);
#ifdef __linux__
		_d.erase(_d.rfind("/"), _d.length());
#else 
		_d.erase(_d.rfind("\\"), _d.length());
#endif 
		zip.add_file_to_zip(buff, _d);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	if (add_scoped_files) {
		zip_single_files();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	__p.one_forward();
	printf(" Done\n");

	zip.close_zip();
}


void upload() {
	files = _parser.get_files_content();
	for (int a = 0; a < (int)files.size(); a++) {
		if (!std::filesystem::exists(files[a])) {
			error_handler::call_error_and_exit("[RUNTIME_ERROR] File path did not exists " + files[a]);
		}
		else if (std::filesystem::is_directory(files[a])) {
			error_handler::call_error_and_exit("[RUNTIME_ERROR] File path was a directory " + files[a]);
		}
	}

	if (_parser.get_dir_path() != "") {
		if (!std::filesystem::exists(_parser.get_dir_path()) || !std::filesystem::is_directory(_parser.get_dir_path())) {
			error_handler::call_error_and_exit("[RUNTIME_ERROR] Directory path did not exists " + _parser.get_dir_path());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		zip_up_directory(_parser.get_dir_path(), _parser.get_file_name(), _parser.include_directory(), (files.size() > 0));
	}
	else {
		zip_prepare(_parser.get_file_name());
		zip_single_files();
		zip.close_zip();
	}

	/*
	Prints all information about the upload, and prompts the user to confirm if all looks OKAY.
	*/
	if (_parser.confirm_curl()) {
		printf("\\===FILES====/\n");
		for (int a = 0; a < (int)all_files.size(); a++) {
			format_file_size f(all_files[a].full_path);
			printf("path: %s, size: %s\n", all_files[a].name.c_str(), f.formatted_size().c_str());
		}
		printf("\\===FILES====/\n");

		format_file_size f(std::filesystem::absolute(_parser.get_file_name()));
		printf("OUTPUT PATH: %s, size: %s\n", std::filesystem::absolute(_parser.get_file_name()).string().c_str(), f.formatted_size().c_str());
		printf("UPLOAD PATH: %s\n", _parser.get_output_path().c_str());

		printf("Does this look good? (Y/N) > ");
		std::string answer;
		std::getline(std::cin, answer);

		std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

		if (answer != "y") {
			printf("OOOK, no then... good bye\n");
			std::filesystem::remove(std::filesystem::absolute(_parser.get_file_name()));
			exit(0);
		}
	}

}

void download() {
	if (_parser.get_file_name() == "") {
		error_handler::call_error_and_exit("[RUNTIME_ERROR] No file name given (ZIP_NAME)");
	}

	if (_parser.confirm_curl()) {
		printf("OUTPUT PATH: %s\n", std::filesystem::absolute(_parser.get_output_path()).string().c_str());

		printf("Does this look good? (Y/N) > ");
		std::string answer;
		std::getline(std::cin, answer);

		std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

		if (answer != "y") {
			printf("OOOK, no then... good bye\n");
			exit(0);
		}
	}
}

int main() {	
	if (!std::filesystem::exists("config.owl")) {
		error_handler::call_error_and_exit("[COMPILER_ERROR]Could not find a config file");
	}
	_parser.parse_config();
	network_handler net;
	if (_parser.upload()) {
		upload();

		if (!net.upload_to_dropbox(std::filesystem::absolute(_parser.get_file_name()), _parser.get_access_token())) {
			error_handler::call_error_and_exit("[RUNTIME_ERROR] Could not send the request to dropbox, check network connection and access token!");
		}
	}
	else if (_parser.download()) {
		download();
		if (!net.download_from_dropbox(_parser.get_file_name(), std::filesystem::absolute(_parser.get_output_path()).string(), _parser.get_access_token()))
			error_handler::call_error_and_exit("[RUNTIME_ERROR] Could not send the request to dropbox, check network connection!");
		else {
			printf("Download successful!\n");
			printf("{\n");
			std::string s = std::filesystem::absolute(_parser.get_output_path()).string() + "/" + _parser.get_file_name();
			printf("OUTPUT: %s\n", s.c_str());
			format_file_size f(s);
			printf("SIZE: %s\n", f.formatted_size().c_str());
			printf("}\n");
			zip.open_zip(s);
			printf("unzipping...");
			zip.unzip(std::filesystem::absolute(_parser.get_output_path()).string());
			std::filesystem::remove(std::filesystem::absolute(_parser.get_output_path()).string() + "/" + _parser.get_file_name());
			printf("Done\n");

		}
	}
	
	if (_parser.confirm_curl()) {
		std::cin.get();
	}
}
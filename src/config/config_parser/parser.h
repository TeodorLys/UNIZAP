#pragma once
#include "lexical_analyzer.h"
#include <vector>
#include <string>
class parser {
	lexical_analyzer lexer;
	std::vector<tokenizer_data> lexer_data;
	std::vector<std::string> files;
	std::vector<std::string> skip;
	std::string access;
	std::string dir_path;
	std::string file_name;
	std::string output_path;
	bool _include_directory; // in the dir_path
	bool _force; //dont do any file size checks or directory failsafes.
	bool _confirm_curl = true;  // Checks with the user if everything are ok.
	bool _upload = false;  // if the user wants to upload a zip file
	bool _download = false; // or download...
public:

	void parse_config();

	/*
	returns a TYPE by name -> see tokens.cpp for all the types available
	@DEPRECATED@
	*/
	std::vector<std::string> get_type(std::string name);


public:
	/*
	These below, are the different parameters you can use in the config file.
	and will be added by the parse_config function.
	*/
	bool include_directory() const { return _include_directory; }
	bool force_upload() const { return _force; }
	bool confirm_curl() const { return _confirm_curl; }
	bool upload() const { return _upload; }
	bool download() const { return _download; }
	std::vector<std::string> get_files_content() { return files; }
	std::vector<std::string> get_skip_files() { return skip; }
	std::string get_access_token() const { return access; }
	std::string get_dir_path() const { return dir_path; }
	std::string get_file_name() const { return file_name; }
	std::string get_output_path() const { return output_path; }

};


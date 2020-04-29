#pragma once
#include "lexical_analyzer.h"
#include <vector>
#include <string>
class parser {
	lexical_analyzer lexer;
	std::vector<tokenizer_data> lexer_data;
	std::vector<std::string> files;
	std::string access;
	std::string dir_path;
	std::string zip_name;
	std::string upload_path;
	bool _include_directory; // in the dir_path
	bool _force; //dont do any file size checks or directory failsafes.
	bool _confirm_curl = true;
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
	std::vector<std::string> get_files_content() { return files; }
	std::string get_access_token() const { return access; }
	std::string get_dir_path() const { return dir_path; }
	std::string get_zip_name() const { return zip_name; }
	std::string get_upload_path() const { return upload_path; }

};


#include "parser.h"
#include <fstream>
#include <string>
#include "string_conversions.h"
#include "../../user_communication/error_handler.h"
#include "../../file_handling/rand_name.h"
#include <filesystem>
#include <iostream>

/*
Another function needing a overhaul. EXTREME JANQ!
*/
void parser::parse_config() {
	std::fstream file(std::filesystem::current_path().string() + "/config.owl", std::fstream::in);
	if (!file.is_open()) {
		error_handler::call_error_and_exit("[COMPILER_ERROR] Could not open confg file, " + std::filesystem::current_path().string() + "/config.owl");
	}

	for (std::string s; getline(file, s);) {
		if (s.find("//") != std::string::npos)
			s.erase(s.find("//"), s.length());
		lexer.tokenize_string(s);
	}
	//lexer._print_tokenize_string();
	lexer_data = *lexer.extract_data();

	for (int a = 0; a < (int)lexer_data.size(); a++) {
		/*
		Here we parse all different string parameters, like ACCESS token and ZIP_NAME
		*/
		if (lexer_data[a + 1].type == tokens::assignment_operators) {
			if(lexer_data[a].value == "ACCESS"){
				if(lexer_data[a + 2].type == tokens::string_literal)
					access = lexer_data[a + 2].value;
				else 
					error_handler::call_error_and_exit("[COMPILER_ERROR] Could not parse the access token");
			}
			else if (lexer_data[a].value == "DIR_PATH") {
				if (lexer_data[a + 2].type == tokens::string_literal)
					dir_path = lexer_data[a + 2].value;
				else
					error_handler::call_error_and_exit("[COMPILER_ERROR] Could not parse the dir_path predef");
			}
			else if (lexer_data[a].value == "FILE_NAME") {
				if (lexer_data[a + 2].type == tokens::string_literal) {
					file_name = lexer_data[a + 2].value;
					if (file_name.find(".zip") == std::string::npos)
						file_name += ".zip";
				}
			}
			else if (lexer_data[a].value == "OUTPUT_PATH") {
				if (lexer_data[a + 2].type == tokens::string_literal)
					output_path = lexer_data[a + 2].value;
				else
					error_handler::call_error_and_exit("[COMPILER_ERROR] Could not parse the output_path predef");
			}
		}/**/
		/*
		This parses the scoped parameters, i.e. parameters with {}, like "FLAGS" and "FILES"
		*/
		else if (lexer_data[a + 1].type == tokens::separators && lexer_data[a + 1].value == "{") {
			int b = a + 2;
			bool syntax_check = false;
			for (int c = b; c < (int)lexer_data.size(); c++)
				if (lexer_data[c].value == "}" && lexer_data[c].value != "{")
					syntax_check = true;

			if (!syntax_check) {
				error_handler::call_error_and_exit("[COMPILE_ERROR]Expected a } at " + lexer_data[b].value);
			}
			if (lexer_data[a].value == "FLAGS") {
				b = a + 2;
				while (lexer_data[b].value != "}" && b < (int)lexer_data.size()) {
					if (lexer_data[b].type == tokens::path_literal) {
						if (lexer_data[b].value == "INCLUDE_DIRECTORY")
							_include_directory = true;
						else if (lexer_data[b].value == "FORCE") {
							_force = true;
						}
						else if (lexer_data[b].value == "DISABLE_CONFIRM_CURL") {
							_confirm_curl = false;
						}
						else if (lexer_data[b].value == "DOWNLOAD") {
							_download = true;
						}
						else if (lexer_data[b].value == "UPLOAD") {
							_upload = true;
						}
					}
					b++;
				}
			}
			else if (lexer_data[a].value == "FILES") {
				b = a + 2;
				while (lexer_data[b].value != "}" && b < (int)lexer_data.size()) {
					if (lexer_data[b].type == tokens::string_literal) {
						files.push_back(lexer_data[b].value);
					}
					b++;
				}
			}
		} /**/
	}

	/*
	If no zip name were given, we just create a random generated one.
	*/
	if (file_name == "" && _upload){
		rand_name r(10);
		std::string t = r.get_name() + ".zip";
		file_name = std::filesystem::absolute(t).string();
		/*
		And if no upload path was given we just use the name, and put it in the most forward folder in dropbox
		*/
		if (output_path == "") {
			std::filesystem::path p(file_name);
			output_path = p.filename().string();
		}
	}
	
	if (output_path == "" && _download) {
		output_path = std::filesystem::current_path().string();
	}
	else if(output_path != ""){
		if (!std::filesystem::exists(output_path))
			error_handler::call_error_and_exit("[PARSER_ERROR] OUTPUT_PATH does not exists!");
	}

	if(access == "")
		error_handler::call_error_and_exit("[RUNTIME_ERROR] Access token was NULL!");

	if (!_upload && !_download)
		error_handler::call_error_and_exit("[RUNTIME_ERROR] Action was unspecified, DOWNLOAD or UPLOAD (in FLAGS)");

}


/*
@DEPRECATED@
*/
std::vector<std::string> parser::get_type(std::string name)  {
	std::vector<std::string> buffer;
	for (int a = 0; a < (int)lexer_data.size(); a++) {
		if (lexer_data[a].type == tokens::type && lexer_data[a].value == name && lexer_data[a + 1].type == tokens::assignment_operators) {
			buffer.push_back(lexer_data[a + 2].value);
			break;
		}
		else if (lexer_data[a].value == name && lexer_data[a + 1].type == tokens::separators && lexer_data[a + 1].value == "{") {
			int b = a + 2;
			bool syntax_check = false;
			for (int c = b; c < (int)lexer_data.size(); c++)
				if (lexer_data[c].value == "}" && lexer_data[c].value != "{")
					syntax_check = true;

			if (!syntax_check) {
				printf("[COMPILE_ERROR]Expected a } at %i\n", lexer_data[b].index);
				std::cin.get();
				exit(0);
			}

			b = a + 2;
			while (lexer_data[b].value != "}" && b < (int)lexer_data.size()) {
				if (lexer_data[b].type == tokens::string_literal || lexer_data[b].type == tokens::path_literal) {
					buffer.push_back(lexer_data[b].value);
				}
				b++;
			}
			break;
		}
	}
	return buffer;
}
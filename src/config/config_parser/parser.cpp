#include "parser.h"
#include <fstream>
#include <string>
#include "string_conversions.h"
#include "error_handler.h"
#include "rand_name.h"
#include <filesystem>
#include <iostream>

/*
Another function needing a overhaul. EXTREME JANQ!
*/
void parser::parse_config() {
	std::fstream file(std::filesystem::current_path().string() + "\\Release\\config.owl", std::fstream::in);
	if (!file.is_open()) {
		error_handler::call_error_and_exit("Could not open config file");
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
					error_handler::call_error_and_exit("Could not parse the access token");
			}
			else if (lexer_data[a].value == "DIR_PATH") {
				if (lexer_data[a + 2].type == tokens::string_literal)
					dir_path = lexer_data[a + 2].value;
				else
					error_handler::call_error_and_exit("Could not parse the dir_path predef");
			}
			else if (lexer_data[a].value == "ZIP_NAME") {
				if (lexer_data[a + 2].type == tokens::string_literal) {
					zip_name = lexer_data[a + 2].value;
					if (zip_name.find(".zip") == std::string::npos)
						zip_name += ".zip";
				}
				else if (lexer_data[a].value == "UPLOAD_PATH") {
					if (lexer_data[a + 2].type == tokens::string_literal)
						upload_path = lexer_data[a + 2].value;
					else
						error_handler::call_error_and_exit("Could not parse the dir_path predef");
				}
				else
					error_handler::call_error_and_exit("Could not parse the dir_path predef");
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
				printf("[COMPILE_ERROR]Expected a } at %s\n", lexer_data[b].value.c_str());
				std::cin.get();
				exit(0);
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
	if (zip_name == ""){
		rand_name r(10);
		std::string t = r.get_name() + ".zip";
		zip_name = std::filesystem::absolute(t).string();
		/*
		And if no upload path was given we just use the name, and put it in the most forward folder in dropbox
		*/
		if (upload_path == "") {
			std::filesystem::path p(zip_name);
			upload_path = p.filename().string();
		}
	}

	if(access == "")
		error_handler::call_error_and_exit("Access token was NULL!");

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
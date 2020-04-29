#include "lexical_analyzer.h"

#include "string_conversions.h"
#include <algorithm>

lexical_analyzer::lexical_analyzer() {
}


void lexical_analyzer::tokenize_string(std::string s) {

	if (s.size() > 0)
		if (s[s.size() - 1] != ' ')
			s += ' ';

	std::string temp;
	bool string_literal_record = false;
	for (char c : s) {
		if (c != ' ' && c != '\"' && !separators::check_type(c) && !string_literal_record) {
			temp += c;
		}
		else if (c == '"') {
			temp += c;
			if (string_literal_record)
				string_literal_record = false;
			else
				string_literal_record = true;
		}
		else if (string_literal_record) {
			temp += c;
		}
		else {
			tokenizer_data buffer;

			if (temp[0] != '"' && temp[temp.size() - 1] != '"')
				temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());

			temp.erase(std::remove(temp.begin(), temp.end(), '\t'), temp.end());

			if (types::check_type(temp)) {
				buffer.type = tokens::type;
				buffer.value = temp;
				tokenized.push_back(buffer);
			}
			/*else if (arithmetic_operators::check_type(temp)) {
				buffer.t = tokens::arithmetic_operators;
				buffer.type = temp;
				tokenized.push_back(buffer);
			}
			else if (rational_operators::check_type(temp)) {
				buffer.t = tokens::rational_operators;
				buffer.type = temp;
				tokenized.push_back(buffer);
			}
			else if (logical_operators::check_type(temp)) {
				buffer.t = tokens::logical_operators;
				buffer.type = temp;
				tokenized.push_back(buffer);
			}
			else if (unary_operators::check_type(temp)) {
				buffer.t = tokens::unary_operators;
				buffer.type = temp;
				tokenized.push_back(buffer);
			}*/
			else if (assignment_operators::check_type(temp)) {
				buffer.type = tokens::assignment_operators;
				buffer.value = temp;
				tokenized.push_back(buffer);
			}
			else if (predefined_literals::check_type(temp)) {
				buffer.type = tokens::path_literal;
				buffer.value = temp;
				tokenized.push_back(buffer);
			}
			else if (string_conversions::char_is_num(temp[0])) {
				bool all_is_num = true;
				for (char v : temp) {
					if (!string_conversions::char_is_num(v)) {
						all_is_num = false;
					}
				}

				if (all_is_num) {
					buffer.type = tokens::string_literal;
					buffer.value = temp;
					tokenized.push_back(buffer);
				}
				else {
					buffer.type = tokens::identifiers;
					buffer.value = temp;
					tokenized.push_back(buffer);
				}
			}
			else if (temp[0] == '"' && temp[temp.size() - 1] == '"') {
				buffer.type = tokens::string_literal;
				temp.pop_back();
				temp.erase(0, 1);
				buffer.value = temp;
				tokenized.push_back(buffer);
			}
		/*	else if (keywords::check_type(temp)) {
				buffer.t = tokens::keywords;
				buffer.type = temp;
				tokenized.push_back(buffer);
			}*/
			else if (temp[0] == '#') {
				temp.clear();
				return;
			}
			else if (temp[0] != 0 && temp[0] != 32) {
				buffer.type = tokens::identifiers;
				buffer.value = temp;
				tokenized.push_back(buffer);
			}

			if (separators::check_type(c)) {
				buffer.type = tokens::separators;
				buffer.value = c;
				tokenized.push_back(buffer);
			}

			temp.clear();
		}
	}

}
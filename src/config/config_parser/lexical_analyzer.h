#pragma once
#include <string>
#include "tokens.h"
#include <vector>

class lexical_analyzer {
private:
	std::vector<tokenizer_data> tokenized;
public:
	lexical_analyzer();
	void tokenize_string(std::string s);

public:

	std::vector<tokenizer_data>* extract_data() { return &tokenized; }

	void _print_tokenize_string() {
		for (int a = 0; a < (int)tokenized.size(); a++) {
			std::string tok = "";
			switch (tokenized[a].type) {
			//case tokens::arithmetic_operators:
			//	tok = "arithmetic_operators";
			//	break;
			case tokens::assignment_operators:
				tok = "assignment_operators";
				break;
			case tokens::comment:
				tok = "comment";
				break;
			case tokens::identifiers:
				tok = "identifiers";
				break;
			//case tokens::keywords:
			//	tok = "keywords";
			//	break;
			//case tokens::literals:
			//	tok = "literals";
			//	break;
			//case tokens::logical_operators:
			//	tok = "logical_operators";
			//	break;
			//case tokens::rational_operators:
			//	tok = "rational_operators";
			//	break;
			case tokens::separators:
				tok = "separators";
				break;
			case tokens::type:
				tok = "type";
				break;
			case tokens::path_literal:
				tok = "path_literal";
				break;
			//case tokens::unary_operators:
			//	tok = "unary_operators";
			//	break;
			case tokens::string_literal:
				tok = "string_literal";
				break;
			}
			printf("{%s, %s}, ", tok.c_str(), tokenized[a].value.c_str()/*, (int)tokenized[a].type[0], tokenized[a].type[0]*/);
		}
		if (tokenized.size() > 0)
			printf("\n");
	}
};


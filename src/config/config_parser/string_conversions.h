#pragma once


class string_conversions {
public:
	static bool char_is_num(char c) {
		switch (c) {
		case '0':
			return true;

		case '1':
			return true;

		case '2':
			return true;

		case '3':
			return true;

		case '4':
			return true;

		case '5':
			return true;

		case '6':
			return true;

		case '7':
			return true;

		case '8':
			return true;

		case '9':
			return true;

		case '.':
			return true;

		default:
			return false;
		}
	}

	static std::string literal_to_type(std::string s) {
		bool has_dot = false;
		for (char c : s) {
			if (!char_is_num(c) && c != '.' && c != ',') {
				return "NULL";
			}
			else if (c == '.' || c == ',') {
				has_dot = true;
			}
		}
		if (has_dot)
			return "FLOAT";
		return "INT";
	}

};

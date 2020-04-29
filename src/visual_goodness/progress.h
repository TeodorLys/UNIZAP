#pragma once
#include <string>
class progress {
private:
	std::string _p;
	int _size;
	int _count;
	int file_p = 1;
	std::string s;
public:
	progress(std::string _s, int size, int file_count) : s(_s), _size(size), _count(file_count) {
		_p.insert(0, size, ' ');
		_p = "[" + _p + "]";
	}

	void one_forward() {
		std::string p = _p;
		float f = (float)file_p / (float)_count;
		int i = (int)(_size * f);
		/*
		This replaces the spaces with #, as progress... pretty self explanatory I think
		*/
		p.replace(p.begin() + 1, p.begin() + i, i, '#');
		/*
		"s +" here adds the progress action ex. Adding files: [#####################]
		because we rewrite the line in console, we need to add the action everytime.
		*/
		p = s + p;
		printf("\r%s", p.c_str());
		file_p++;
	}

};


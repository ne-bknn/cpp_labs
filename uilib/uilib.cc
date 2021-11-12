#include <iostream>
#include <limits>

#include "uilib.h"

namespace ui {
	std::string get_str() {
		std::string a;
		while (! (std::cin >> a)) {
			if (std::cin.eof()) {
				throw std::runtime_error("EOF");
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		return a;
	}

	std::string get_str_custom(std::istream& is) {
		std::string a;
		while (! (is >> a)) {
			if (is.eof()) {
				throw std::runtime_error("EOF");
			}
			is.clear();
			is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		return a;
	}
}

#include <iostream>
#include <string>
#include "uilib.h"

int main() {
	int a;
	try {
		ui::get(a);
	} catch (const std::runtime_error& e) {
		std::cerr << "[-] Caught EOF" << std::endl;
		std::exit(1);
	}

	std::cout << a << std::endl;

	std::string b;
	try {
		b = ui::get_str();
	} catch (const std::runtime_error& e) {
		std::cerr << "[-] Caught EOF" << std::endl;
	}

	std::cout << b << std::endl;
}

#include <iostream>

#include "../uilib/uilib.h"
#include "curve.h"

int main() {
    double r = 0;
    while (r <= 0) {
	std::cout
	    << "Enter curve's radius. Radius cannot be negative or zero: ";
	try {
		ui::get(r);
	} catch (std::runtime_error&) {
	    std::cout << "[-] Caught EOF, exiting" << std::endl;
	    break;
	}
    }

    Nephroid curve(r);

    while (true) {
	std::cout << "$ ";
	std::string cmd;
	try {
	    cmd = ui::get_str();
	} catch (std::runtime_error&) {
	    std::cout << "[-] Caught EOF, exiting" << std::endl;
	    break;
	}

	if (cmd.compare(std::string{"arclength"}) == 0) {
	    std::cout << curve.arclength() << std::endl;
	} else if (cmd.compare(std::string{"curvature"}) == 0) {
	    double t;
	    try {
		ui::get(t);
	    } catch (std::runtime_error&) {
		std::cout << "[-] Caught EOF, exiting" << std::endl;
		break;
	    }
	    std::cout << curve.curvature_radius(t) << std::endl;
	} else if (cmd.compare(std::string{"radius"}) == 0) {
	    std::cout << curve.r() << std::endl;
	} else if (cmd.compare(std::string{"area"}) == 0) {
	    std::cout << curve.area() << std::endl;
	} else if (cmd.compare(std::string{"x_coord"}) == 0) {
	    double t;
	    try {
		ui::get(t);
	    } catch (std::runtime_error&) {
		std::cout << "[-] Caught EOF, exiting" << std::endl;
		break;
	    }

	    std::cout << curve.x(t) << std::endl;
	} else if (cmd.compare(std::string{"y_coord"}) == 0) {
	    double t;
	    try {
		ui::get(t);
	    } catch (std::runtime_error&) {
		std::cout << "[-] Caught EOF, exiting" << std::endl;
		break;
	    }

	    std::cout << curve.y(t) << std::endl;
	} else if (cmd.compare(std::string{"repr"}) == 0) {
	    std::cout << curve.repr() << std::endl;
	} else if (cmd.compare(std::string{"set"}) == 0) {
	    double t;
	    try {
		ui::get(t);
	    } catch (std::runtime_error&) {
		std::cout << "[-] Caught EOF, exiting" << std::endl;
		break;
	    }

	    try {
		curve.set_r(t);
		std::cout << "Radius is set to " << t << std::endl;
	    } catch (std::domain_error&) {
		std::cout << "[-] Radius must be non-zero positive number"
		          << std::endl;
	    }
	} else if (cmd.compare(std::string{"help"}) == 0) {
	    using std::endl;
	    std::cout << "radius" << endl
	              << "arclength" << endl
	              << "curvature <t>" << endl
	              << "area" << endl
	              << "x_coord <t>" << endl
	              << "y_coord <t>" << endl
	              << "repr" << endl
	              << "help" << endl;
	} else {
	    std::cout << "There's no such command. Use help to view available "
	                 "commands"
	              << std::endl;
	}
    }
}

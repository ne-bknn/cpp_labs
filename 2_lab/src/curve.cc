#include "curve.h"

#include <math.h>

#include <iostream>
#include <string>
#include <string.h>

Nephroid::Nephroid() {
   this->_r = 1;
   this->_repr = nullptr;
   this->_repr_len = 0;
}

Nephroid::Nephroid(double r) {
    this->_r = r;
    this->_repr = nullptr;
    this->_repr_len = 0;
}

Nephroid::Nephroid(const Nephroid &obj) noexcept {
    this->_r = obj._r;
    this->_repr = new char[obj._repr_len];
    strncpy(this->_repr, obj._repr, obj._repr_len);
    this->_repr_len = obj._repr_len;
}

Nephroid& Nephroid::operator=(const Nephroid &obj) noexcept {
    if (this != &obj) {
	    this->_r = obj._r;
	    if (obj._repr != nullptr) {
	    	this->_repr = new char[obj._repr_len];
    	    	strncpy(this->_repr, obj._repr, obj._repr_len);
	    } else {
		this->_repr = nullptr;
	    }

	    this->_repr_len = obj._repr_len;
    }
    return *this;
}

Nephroid::~Nephroid() {
    if (this->_repr != nullptr) {
	delete[] this->_repr;
    }
}

void Nephroid::set_r(double r) {
    if (r <= 0) {
	throw std::domain_error("Radius must be positive value");
    }

    this->_r = r;
    delete[] this->_repr;
    this->_repr = nullptr;
    this->_repr_len = 0;
}

double Nephroid::r() { return this->_r; }

double Nephroid::R() { return this->_r * 2; }

double Nephroid::arclength() { return this->_r * 24; }

double Nephroid::area() { return 12 * M_PI * this->_r * this->_r; }

double Nephroid::curvature_radius(double t) {
    return abs(3 * this->_r * sin(t));
}

double Nephroid::x(double t) {
    return 6 * this->_r * cos(t) - 4 * this->_r * sin(t);
}

double Nephroid::y(double t) { return 4 * this->_r * pow(sin(t), 3); }

char* Nephroid::repr() {
    if (this->_repr == nullptr) {
	double coef_a = 4 * pow(this->_r, 2);
	double coef_a_length = log10(coef_a) + 10;
	double coef_b = 108 * pow(this->_r, 4);
	double coef_b_legnth = log10(coef_b) + 10;
	double buf_len = coef_a_length + coef_b_legnth + 20;

	char* repr = new char[buf_len];
	sprintf(repr, "(x^2+y^2-%0.3f)^3=%0.3f*x^4", coef_a, coef_b);
	this->_repr = repr;
	this->_repr_len = buf_len;
    }
    return this->_repr;
}

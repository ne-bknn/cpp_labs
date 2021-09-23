#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

class Nephroid {
        private:
                double _r;
        public:
                double r();
                double R();

                double arclength();
                double area();
                double curvature_radius(double t);
                double x(double t);
                double y(double t);

                std::string repr();
};

double Nephroid::r() {
        return this->_r;
}

double Nephroid::R() {
        return this->_r*2;
}

double Nephroid::arclength() {
        return this->_r*24;
}

double Nephroid::area() {
        return 12*M_PI*this->_r*this->_r;
}

double Nephroid::curvature_radius(double t) {
        return abs(3*this->_r*sin(t));
}

double Nephroid::x(double t) {
        return 6*this->_r*cos(t)-4*this->_r*sin(t);
}

double Nephroid::y(double t) {
        return 4*this->_r*pow(sin(t), 3);
}

std::string Nephroid::repr() {
        double coef_a = 4*pow(this->_r, 2);
        double coef_a_length = log10(coef_a)+10;
        double coef_b = 108*pow(this->_r, 4);
        double coef_b_legnth = log10(coef_b)+10;
        double buf_len = coef_a_length+coef_b_legnth+20;

        char* repr = new char[buf_len];
        int n = sprintf(repr, "(x^2+y^2-%0.3f)^3=%0.3f*x^4", coef_a, coef_b);
        return repr;
}

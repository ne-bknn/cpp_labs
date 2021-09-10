#include <iostream>

#include "lib.h"
#include "structures.h"

int main() {
    Structures::CSRMatrix<int> matrix(10, 10);
    // 0 1 0 1 0 1 0 1 0 1
    for (int i = 0; i < 10; ++i) {
	matrix.push(0, i % 2);
    }

    for (int i = 0; i < 10; ++i) {
	std::cout << matrix.get(0, i) << " "
	          << " " << i % 2 << std::endl;
    }
}

#include <iostream>

#include "lib.h"
#include "structures.h"

int main() {
    Structures::Vector<int> a;
    a.push(2);

    int k = a.get(0);

    std::cout << k << std::endl;
    return 0;
}

#include <iostream>
#include "LongMath.hpp"

int main() {
    LongNumber a(5.0, 4);  // 5.5 = 101.1 в двоичной системе (с precision=4)
    LongNumber b(2.5, 4); // 2.25 = 10.01 в двоичной системе
    std::cout << "a = " << a.toString() << std::endl;       // -101.1000
    std::cout << "b = " << b.toString() << std::endl;       // 10.0100
    std::cout << "a + b = " << (a + b).toString() << std::endl; // 111.1100 (7.75)
    std::cout << "a - b = " << (a - b).toString() << std::endl; // 11.0100 (3.25)
    std::cout << "a * b = " << (a * b).toString() << std::endl; // 1100.0110 (12.375)
    std::cout << "a / b = " << (a / b).toString() << std::endl; // 10.0110 (примерно 2.4375)

    return 0;
}
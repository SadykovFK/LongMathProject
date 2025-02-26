#include <iostream>
#include "LongMathBits.hpp"

int main() {
    LongNumber a(5.0, 4);  
    LongNumber b(2.5, 4);
    LongNumber c = 10.5_longnum;
    std::cout << "a = " << a.toString() << std::endl;       
    std::cout << "b = " << b.toString() << std::endl;
    std::cout << "c = " << c.toString() << std::endl;
    std::cout << "a + b = " << (a + b).toString() << std::endl; 
    std::cout << "a - b = " << (a - b).toString() << std::endl; 
    std::cout << "a * b = " << (a * b).toString() << std::endl; 
    std::cout << "a / b = " << (a / b).toString() << std::endl; 
    return 0;
}
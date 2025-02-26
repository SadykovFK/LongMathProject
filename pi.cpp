#include "LongMathBits.hpp"
#include <iostream>

int main() {

    int iterations = 100;
    int precision = 400;

    LongNumber pi(0.0, precision);           
    LongNumber four(4.0, precision);         
    LongNumber denominator(1.0, precision);  
    LongNumber term(0.0, precision);         
    bool add = true;                         

    for (int i = 0; i < iterations; ++i) {
        term = four / denominator;

        if (add) {
            pi = pi + term;
        } else {
            pi = pi - term;
        }

        denominator = denominator + LongNumber(2.0, precision);

        add = !add;
    }

    pi.setPrecision(precision);

    std::cout << "Число Пи с " << iterations << " итерациями и точностью " << precision << " знаков:\n";
    std::cout << pi.toString() << std::endl;

    return 0;
}
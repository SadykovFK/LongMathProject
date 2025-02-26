#include "LongMathBits.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char *argv[]) {

    auto start_time = std::chrono::steady_clock::now();

    int precision = std::stoi(argv[1]);

    LongNumber pi(0, precision);
    LongNumber n0{1, precision};
    LongNumber n(16, precision);

    LongNumber num1{4, precision};
    LongNumber num2{2, precision};
    LongNumber num3{1, precision};
    LongNumber num4{1, precision};

    LongNumber num1_add(1, precision);
    LongNumber num2_add(4, precision);
    LongNumber num3_add(5, precision);
    LongNumber num4_add(6, precision);

    LongNumber add(8, precision);

    for (auto i = 0; i < precision; ++i)
    {
        pi = pi + n0 * (num1 / num1_add - num2 / num2_add - num3 / num3_add - num4 / num4_add);
        n0 = n0 / n;
        num1_add = num1_add + add;
        num2_add = num2_add + add;
        num3_add = num3_add + add;
        num4_add = num4_add + add;
    }

    pi.setPrecision(precision);

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Время = "  << duration.count() << " мс\n";
    std::cout << pi.toString() << std::endl;

    return 0;
}
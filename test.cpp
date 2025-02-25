#include <iostream>
#include <string>
#include "LongMathBits.hpp"


bool checkResult(const LongNumber& actual, const std::string& expectedBinary) {
    return (actual.toString() == expectedBinary);
}

// Вывод verdict: OK или FAIL
void printVerdict(bool condition, const std::string& testName) {
    std::cout << testName << ": " << (condition ? "OK" : "FAIL") << std::endl;
}

int runTests() {
    int failCount = 0;

    // 1) Тест сложения
    {
        LongNumber a(5.0, 4); 
        LongNumber b(2.0, 4); 
        LongNumber res = a + b; 
        bool ok = (res.toString() == "111.0000");
        printVerdict(ok, "Addition test");
        if(!ok) failCount++;
    }

    // 2) Тест вычитания
    {
        LongNumber a(5.0, 4); 
        LongNumber b(2.0, 4); 
        LongNumber res = a - b; 
        bool ok = (res.toString() == "11.0000");
        printVerdict(ok, "Subtraction test");
        if(!ok) failCount++;
    }

    // 3) Тест умножения
    {
        LongNumber a(3.0, 4); // 3.0000
        LongNumber b(2.0, 4); // 2.0000
        LongNumber res = a * b;
        // 3 * 2 = 6; двоично → "110.0000" 
        bool ok = (res.toString() == "110.0000");
        printVerdict(ok, "Multiplication test");
        if(!ok) failCount++;
    }

    // 4) Тест деления
    {
        LongNumber a(5.0, 4); 
        LongNumber b(2.0, 4);
        LongNumber res = a / b;
        bool ok = (res.toString() == "10.1000");
        printVerdict(ok, "Division test");
        if(!ok) failCount++;
    }

    // 5) Тест установки precision
    {
        LongNumber a(5.75, 4);
        a.setPrecision(2);
        bool ok = (a.toString() == "101.11");
        printVerdict(ok, "SetPrecision test");
        if(!ok) failCount++;
    }

    return failCount;
}

int main()
{
    runTests();
    return 0;
}

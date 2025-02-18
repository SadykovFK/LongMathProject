#include "LongMathBits.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <iostream>

LongNumber::LongNumber() : precision(10), isNegative(false)
{}

LongNumber::LongNumber(long double number, int precision) : precision(precision)
{
    isNegative = number < 0;
    number = std::abs(number);
    toBinary(number);
}

LongNumber::LongNumber(const LongNumber& other)
{
    integerPart = other.integerPart;
    fractionalPart = other.fractionalPart;
    precision = other.precision;
    isNegative = other.isNegative;
}

LongNumber::~LongNumber()
{}

LongNumber& LongNumber::operator=(const LongNumber& other)
{
    if (this != &other) {
        integerPart = other.integerPart;
        fractionalPart = other.fractionalPart;
        precision = other.precision;
        isNegative = other.isNegative;
    }
    return *this;
}

void LongNumber::toBinary(long double number)
{
    integerPart.clear();
    fractionalPart.clear();

    long long intPart = static_cast<long long>(number);
    while (intPart > 0) {
        integerPart.insert(integerPart.begin(), intPart % 2);
        intPart /= 2;
    }

    double fracPart = number - static_cast<long long>(number);
    for (int i = 0; i < precision; ++i) {
        fracPart *= 2;
        fractionalPart.push_back(static_cast<int>(fracPart));
        fracPart -= static_cast<int>(fracPart);
    }
}

long double LongNumber::toDouble() const
{
    long double result = 0.0;

    for (size_t i = 0; i < integerPart.size(); ++i) {
        result += integerPart[i] * std::pow(2, integerPart.size() - i - 1);
    }

    for (size_t i = 0; i < fractionalPart.size(); ++i) {
        result += fractionalPart[i] * std::pow(2, -(i + 1));
    }

    return isNegative ? -result : result;
}

void LongNumber::alignPrecision(LongNumber& other)
{
    while (precision < other.precision) {
        fractionalPart.push_back(false);
        ++precision;
    }
    while (other.precision < precision) {
        other.fractionalPart.push_back(false);
        ++other.precision;
    }
}

void LongNumber::shiftLeft(size_t count)
{
    for (size_t j = 0; j < count; ++j) {
        if (!integerPart.empty()) {
            fractionalPart.insert(fractionalPart.begin(), integerPart.back());
            integerPart.pop_back();
        } else {
            fractionalPart.insert(fractionalPart.begin(), false);
        }
    }
}

void LongNumber::shiftRight(size_t count)
{
    for (size_t j = 0; j < count; ++j) {
        if (!fractionalPart.empty()) {
            integerPart.push_back(fractionalPart.front());
            fractionalPart.erase(fractionalPart.begin());
        } else {
            integerPart.push_back(false);
        }
    }
}

bool LongNumber::operator<(const LongNumber& other) const
{
    if (isNegative != other.isNegative) {
        return isNegative;
    }
    if (integerPart.size() != other.integerPart.size()) {
        return integerPart.size() < other.integerPart.size();
    }
    for (size_t i = 0; i < integerPart.size(); ++i) {
        if (integerPart[i] != other.integerPart[i]) {
            return integerPart[i] < other.integerPart[i];
        }
    }
    for (size_t i = 0; i < fractionalPart.size(); ++i) {
        if (fractionalPart[i] != other.fractionalPart[i]) {
            return fractionalPart[i] < other.fractionalPart[i];
        }
    }
    return false;
}

bool LongNumber::operator>(const LongNumber& other) const
{
    return other < *this;
}

bool LongNumber::operator>=(const LongNumber& other) const
{
    return !(*this < other);
}

LongNumber LongNumber::operator+(const LongNumber& other) const
{
    LongNumber result;
    result.precision = std::max(precision, other.precision);
    result.isNegative = isNegative;

    LongNumber thisCopy = *this;
    LongNumber otherCopy = other;
    thisCopy.alignPrecision(otherCopy);

    bool carry = false;
    size_t maxSize = std::max(thisCopy.integerPart.size(), otherCopy.integerPart.size());
    for (size_t i = 0; i < maxSize; ++i) {
        bool a = (i < thisCopy.integerPart.size()) ? thisCopy.integerPart[i] : false;
        bool b = (i < otherCopy.integerPart.size()) ? otherCopy.integerPart[i] : false;
        bool sum = a ^ b ^ carry;
        carry = (a && b) || (a && carry) || (b && carry);
        result.integerPart.push_back(sum);
    }
    if (carry) {
        result.integerPart.push_back(true);
    }

    carry = false;
    for (int i = thisCopy.fractionalPart.size() - 1; i >= 0; --i) {
        bool a = thisCopy.fractionalPart[i];
        bool b = otherCopy.fractionalPart[i];
        bool sum = a ^ b ^ carry;
        carry = (a && b) || (a && carry) || (b && carry);
        result.fractionalPart.push_back(sum);
    }

    return result;
}

LongNumber LongNumber::operator-(const LongNumber& other) const
{
    LongNumber result;
    result.precision = std::max(precision, other.precision);

    LongNumber thisCopy = *this;
    LongNumber otherCopy = other;
    thisCopy.alignPrecision(otherCopy);

    if (thisCopy < otherCopy) {
        std::swap(thisCopy, otherCopy);
        result.isNegative = true;
    } else {
        result.isNegative = false;
    }

    bool borrow = false;
    size_t maxSize = std::max(thisCopy.integerPart.size(), otherCopy.integerPart.size());
    for (size_t i = 0; i < maxSize; ++i) {
        bool a = (i < thisCopy.integerPart.size()) ? thisCopy.integerPart[i] : false;
        bool b = (i < otherCopy.integerPart.size()) ? otherCopy.integerPart[i] : false;
        bool diff = a ^ b ^ borrow;
        borrow = (!a && b) || (!a && borrow) || (b && borrow);
        result.integerPart.push_back(diff);
    }

    borrow = false;
    for (int i = thisCopy.fractionalPart.size() - 1; i >= 0; --i) {
        bool a = thisCopy.fractionalPart[i];
        bool b = otherCopy.fractionalPart[i];
        bool diff = a ^ b ^ borrow;
        borrow = (!a && b) || (!a && borrow) || (b && borrow);
        result.fractionalPart.push_back(diff);
    }

    return result;
}

LongNumber LongNumber::operator*(const LongNumber& other) const
{
    LongNumber result;
    result.precision = precision + other.precision;

    for (size_t i = 0; i < integerPart.size(); ++i) {
        if (integerPart[i]) {
            LongNumber temp = other;
            temp.shiftLeft(i);
            result = result + temp;
        }
    }

    for (size_t i = 0; i < fractionalPart.size(); ++i) {
        if (fractionalPart[i]) {
            LongNumber temp = other;
            temp.shiftRight(i + 1);
            result = result + temp;
        }
    }

    result.isNegative = isNegative != other.isNegative;
    return result;
}

LongNumber LongNumber::operator/(const LongNumber& other) const
{
    if (other.integerPart.empty() && other.fractionalPart.empty()) {
        throw std::invalid_argument("Division by zero!");
    }

    LongNumber dividend = *this;
    LongNumber divisor = other;
    LongNumber result;
    result.precision = precision;
    result.isNegative = isNegative != other.isNegative;

    dividend.isNegative = false;
    divisor.isNegative = false;

    dividend.alignPrecision(divisor);

    while (dividend >= divisor) {
        dividend = dividend - divisor;
        result.integerPart.push_back(true);
    }

    for (int i = 0; i < precision; ++i) {
        dividend.shiftLeft();
        if (dividend >= divisor) {
            dividend = dividend - divisor;
            result.fractionalPart.push_back(true);
        } else {
            result.fractionalPart.push_back(false);
        }
    }

    return result;
}

bool LongNumber::operator==(const LongNumber& other) const
{
    return integerPart == other.integerPart && fractionalPart == other.fractionalPart && isNegative == other.isNegative;
}

bool LongNumber::operator!=(const LongNumber& other) const
{
    return !(*this == other);
}

void LongNumber::setPrecision(int newPrecision)
{
    if (newPrecision >= precision) {
        precision = newPrecision;
        return;
    }

    int cutoffIndex = fractionalPart.size() - (precision - newPrecision);

    if (cutoffIndex < fractionalPart.size() && cutoffIndex >= 0) {
        bool carry = fractionalPart[cutoffIndex];
        fractionalPart.resize(cutoffIndex);

        for (int i = cutoffIndex - 1; i >= 0 && carry; --i) {
            fractionalPart[i] = !fractionalPart[i];
            carry = !fractionalPart[i];
        }

        if (carry) {
            integerPart.push_back(true);
        }
    }

    precision = newPrecision;
}

LongNumber operator""_longnum(long double number)
{
    return LongNumber(number);
}

std::string LongNumber::toString() const
{
    std::ostringstream oss;
    if (isNegative) {
        oss << "-";
    }
    for (bool bit : integerPart) {
        oss << bit;
    }
    oss << ".";
    for (bool bit : fractionalPart) {
        oss << bit;
    }
    return oss.str();
}

void LongNumber::normalize()
{
    while (!integerPart.empty() && integerPart.front() == false) {
        integerPart.erase(integerPart.begin());
    }
    if (integerPart.empty()) {
        integerPart.push_back(false);
    }

    while (!fractionalPart.empty() && fractionalPart.back() == false) {
        fractionalPart.pop_back();
    }
    if (fractionalPart.empty()) {
        fractionalPart.push_back(false);
    }
}
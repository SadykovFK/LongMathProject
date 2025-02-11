#include "LongMath.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>

LongNumber::LongNumber() : precision(10), isNegative(false)
{}

LongNumber::LongNumber(long double number, int precision) : precision(precision)
{
    isNegative = number < 0;
    number = std::abs(number);

    long double integerPart = std::floor(number);
    long double fractionalPart = number - integerPart;

    while (integerPart >= 1) {
        digits.insert(digits.begin(), static_cast<int>(std::fmod(integerPart, 10)));
        integerPart = std::floor(integerPart / 10);
    }

    for (int i = 0; i < precision; ++i) {
        fractionalPart *= 10;
        digits.push_back(static_cast<int>(std::floor(fractionalPart)));
        fractionalPart -= std::floor(fractionalPart);
    }

    normalize();
}

LongNumber::LongNumber(const LongNumber& other)
{
    digits = other.digits;
    precision = other.precision;
    isNegative = other.isNegative;
}

LongNumber::~LongNumber()
{}

LongNumber& LongNumber::operator=(const LongNumber& other)
{
    if (this != &other) {
        digits = other.digits;
        precision = other.precision;
        isNegative = other.isNegative;
    }
    return *this;
}

void LongNumber::alignPrecision(LongNumber& other)
{
    while (precision < other.precision) {
        digits.push_back(0);
        ++precision;
    }
    while (other.precision < precision) {
        other.digits.push_back(0);
        ++other.precision;
    }
}

LongNumber LongNumber::operator+(const LongNumber& other) const
{
    LongNumber result = *this;
    LongNumber otherCopy = other;
    result.alignPrecision(otherCopy);

    if (result.isNegative == otherCopy.isNegative) {
        int carry = 0;
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            int sum = result.digits[i] + otherCopy.digits[i] + carry;
            result.digits[i] = sum % 10;
            carry = sum / 10;
        }
        if (carry) {
            result.digits.insert(result.digits.begin(), carry);
        }
    } else {
        if (result.isNegative) {
            result.isNegative = false;
            return otherCopy - result;
        } else {
            otherCopy.isNegative = false;
            return result - otherCopy;
        }
    }
    return result;
}

LongNumber LongNumber::operator-(const LongNumber& other) const
{
    LongNumber result = *this;
    LongNumber otherCopy = other;
    result.alignPrecision(otherCopy);

    if (result.isNegative == otherCopy.isNegative) {
        if (result < otherCopy) {
            LongNumber temp = result;
            result = otherCopy;
            otherCopy = temp;
            result.isNegative = !result.isNegative;
        }
        int borrow = 0;
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            int diff = result.digits[i] - otherCopy.digits[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits[i] = diff;
        }
    } else {
        otherCopy.isNegative = result.isNegative;
        return result + otherCopy;
    }
    return result;
}

LongNumber LongNumber::operator*(const LongNumber& other) const
{
    LongNumber result;
    result.precision = precision + other.precision;
    result.digits.resize(digits.size() + other.digits.size(), 0);

    for (int i = digits.size() - 1; i >= 0; --i) {
        for (int j = other.digits.size() - 1; j >= 0; --j) {
            int product = digits[i] * other.digits[j] + result.digits[i + j + 1];
            result.digits[i + j + 1] = product % 10;
            result.digits[i + j] += product / 10;
        }
    }

    result.isNegative = isNegative != other.isNegative;
    result.normalize();
    return result;
}

LongNumber LongNumber::operator/(const LongNumber& other) const
{
    if (other.digits.empty() || (other.digits.size() == 1 && other.digits[0] == 0)) {
        throw std::invalid_argument("Division by 0!");
    }

    LongNumber dividend = *this;
    LongNumber divisor = other;
    LongNumber result;
    result.precision = precision;
    result.isNegative = isNegative != other.isNegative;

    dividend.isNegative = false;
    divisor.isNegative = false;

    dividend.alignPrecision(divisor);

    result.digits.resize(dividend.digits.size(), 0);

    LongNumber currentDividend;
    for (size_t i = 0; i < dividend.digits.size(); ++i) {
        currentDividend.digits.push_back(dividend.digits[i]);
        currentDividend.normalize();

        int count = 0;
        while (currentDividend > divisor) {
            currentDividend = currentDividend - divisor;
            ++count;
        }

        result.digits[i] = count;
    }

    result.normalize();
    return result;
}

bool LongNumber::operator==(const LongNumber& other) const
{
    return digits == other.digits && isNegative == other.isNegative;
}

bool LongNumber::operator!=(const LongNumber& other) const
{
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const
{
    if (isNegative != other.isNegative) {
        return isNegative;
    }
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size();
    }
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i];
        }
    }
    return false;
}

bool LongNumber::operator>(const LongNumber& other) const
{
    return other < *this;
}

void LongNumber::setPrecision(int newPrecision)
{
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
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i == digits.size() - precision) {
            oss << ".";
        }
        oss << digits[i];
    }
    return oss.str();
}

void LongNumber::normalize()
{
    while (!digits.empty() && digits.front() == 0) {
        digits.erase(digits.begin());
    }
    if (digits.empty()) {
        digits.push_back(0);
    }
}
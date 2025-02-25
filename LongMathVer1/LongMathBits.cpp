#include "LongMathBits.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>

LongNumber::LongNumber() : precision(10), isNegative(false) {
    digits.push_back('0');
}

LongNumber::LongNumber(long double number, int precision) : precision(precision) {
    isNegative = number < 0;
    number = std::abs(number);

    long long integerPart = static_cast<long long>(number);
    if (integerPart == 0) {
        digits.push_back('0');
    } else {
        while (integerPart > 0) {
            digits.insert(digits.begin(), '0' + (integerPart % 2));
            integerPart /= 2;
        }
    }

    double fractionalPart = number - static_cast<long long>(number);
    size_t originalSize = digits.size(); 
    for (int i = 0; i < precision && fractionalPart > 0; ++i) {
        fractionalPart *= 2;
        int bit = static_cast<int>(fractionalPart);
        digits.push_back('0' + bit);
        fractionalPart -= bit;
    }

    while (digits.size() - originalSize < precision) {
        digits.push_back('0');
    }

    normalize();
}

LongNumber::LongNumber(const LongNumber& other) {
    digits = other.digits;
    precision = other.precision;
    isNegative = other.isNegative;
}

LongNumber::~LongNumber() {}

LongNumber& LongNumber::operator=(const LongNumber& other) {
    if (this != &other) {
        digits = other.digits;
        precision = other.precision;
        isNegative = other.isNegative;
    }
    return *this;
}

void LongNumber::normalize() {
    size_t integerPartSize = digits.size() - precision;
    while (integerPartSize > 1 && digits.front() == '0') {
        digits.erase(digits.begin());
        --integerPartSize;
    }
    if (digits.empty()) {
        digits.push_back('0');
        precision = 0;
        isNegative = false;
    }

    while (digits.size() - (digits.size() - precision) < precision) {
        digits.push_back('0');
    }
}

void LongNumber::alignPrecision(LongNumber& other) {
    size_t totalBitsThis = digits.size();
    size_t totalBitsOther = other.digits.size();

    while (totalBitsThis < totalBitsOther) {
        digits.insert(digits.begin(), '0');
        ++totalBitsThis;
    }
    while (totalBitsOther < totalBitsThis) {
        other.digits.insert(other.digits.begin(), '0');
        ++totalBitsOther;
    }

    while (precision < other.precision) {
        digits.push_back('0');
        ++precision;
    }
    while (other.precision < precision) {
        other.digits.push_back('0');
        ++other.precision;
    }
}

LongNumber LongNumber::abs() const {
    LongNumber result = *this;
    result.isNegative = false;
    return result;
}

LongNumber LongNumber::operator+(const LongNumber& other) const {
    LongNumber result = *this;
    LongNumber otherCopy = other;
    result.alignPrecision(otherCopy);

    if (result.isNegative == otherCopy.isNegative) {
        int carry = 0;
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            int sum = (result.digits[i] - '0') + (otherCopy.digits[i] - '0') + carry;
            result.digits[i] = '0' + (sum % 2);
            carry = sum / 2;
        }
        if (carry) {
            result.digits.insert(result.digits.begin(), '0' + carry);
        }
    } else {
        if (result.isNegative) {
            return otherCopy - result.abs();
        } else {
            return result - otherCopy.abs();
        }
    }
    result.normalize();
    return result;
}

LongNumber LongNumber::operator-(const LongNumber& other) const {
    LongNumber result = *this;
    LongNumber otherCopy = other;
    result.alignPrecision(otherCopy);

    if (result.isNegative == otherCopy.isNegative) {
        if (result < otherCopy) {
            LongNumber temp = otherCopy - result;
            temp.isNegative = true;
            return temp;
        }
        int borrow = 0;
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            int diff = (result.digits[i] - '0') - (otherCopy.digits[i] - '0') - borrow;
            if (diff < 0) {
                diff += 2;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits[i] = '0' + diff;
        }
    } else {
        otherCopy.isNegative = !otherCopy.isNegative;
        return result + otherCopy;
    }
    result.normalize();
    return result;
}

LongNumber LongNumber::operator*(const LongNumber& other) const {
    LongNumber result;
    result.precision = precision + other.precision;
    result.digits.resize(digits.size() + other.digits.size(), '0');

    LongNumber temp = *this;
    temp.isNegative = false;
    LongNumber otherCopy = other;
    otherCopy.isNegative = false;

    for (int i = otherCopy.digits.size() - 1; i >= 0; --i) {
        if (otherCopy.digits[i] == '1') {
            LongNumber shifted = temp;
            for (int s = 0; s < otherCopy.digits.size() - 1 - i; ++s) {
                shifted.digits.push_back('0');
            }
            result = result + shifted;
        }
    }

    result.isNegative = isNegative != other.isNegative;
    result.normalize();
    return result;
}

LongNumber LongNumber::operator/(const LongNumber& other) const {
    if (other.digits.size() == 1 && other.digits[0] == '0') {
        throw std::invalid_argument("Division by 0!");
    }

    LongNumber dividend = this->abs();
    LongNumber divisor = other.abs();

    LongNumber intDividend = dividend;
    intDividend.precision = 0;
    LongNumber intDivisor = divisor;
    intDivisor.precision = 0;

    int targetPrecision = precision;
    for (int i = 0; i < targetPrecision; i++) {
        intDividend.digits.push_back('0');
    }

    int n = intDividend.digits.size();
    int m = intDivisor.digits.size();
    int L = n - m + 1;
    if (L < 1) L = 1;

    LongNumber R = intDividend;
    R.precision = 0;
    std::vector<char> Qdigits(L, '0');

    auto shiftLeft = [&](const LongNumber &num, int shift) -> LongNumber {
        LongNumber res = num;
        for (int i = 0; i < shift; i++) {
            res.digits.push_back('0');
        }
        res.precision = 0;
        res.normalize();
        return res;
    };

    for (int i = L - 1; i >= 0; i--) {
        LongNumber shiftedDivisor = shiftLeft(intDivisor, i);
        if (R >= shiftedDivisor) {
            R = R - shiftedDivisor;
            Qdigits[L - 1 - i] = '1';
        }
    }

    while (Qdigits.size() > 1 && Qdigits[0] == '0') {
        Qdigits.erase(Qdigits.begin());
    }

    LongNumber quotient;
    quotient.digits = Qdigits;
    quotient.precision = targetPrecision; 
    quotient.isNegative = (this->isNegative != other.isNegative);
    quotient.normalize();
    return quotient;

}

bool LongNumber::operator==(const LongNumber& other) const {
    return digits == other.digits && isNegative == other.isNegative;
}

bool LongNumber::operator!=(const LongNumber& other) const {
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const {
    if (isNegative != other.isNegative) return isNegative;
    if (digits.size() != other.digits.size()) return digits.size() < other.digits.size();
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) return digits[i] < other.digits[i];
    }
    return false;
}

bool LongNumber::operator>(const LongNumber& other) const {
    return other < *this;
}

bool LongNumber::operator>=(const LongNumber& other) const {
    return !(*this < other);
}

void LongNumber::setPrecision(int newPrecision) {
    if (newPrecision >= precision) {
        digits.resize(digits.size() + (newPrecision - precision), '0');
        precision = newPrecision;
        return;
    }

    int cutoffIndex = digits.size() - precision + newPrecision;
    if (cutoffIndex < digits.size() && cutoffIndex >= 0) {
        int carry = (digits[cutoffIndex] - '0' >= 1) ? 1 : 0;
        digits.resize(cutoffIndex);

        for (int i = cutoffIndex - 1; i >= 0 && carry > 0; --i) {
            int temp = (digits[i] - '0') + carry;
            digits[i] = '0' + (temp % 2);
            carry = temp / 2;
        }

        if (carry > 0) {
            digits.insert(digits.begin(), '0' + carry);
        }
    }
    precision = newPrecision;
    normalize();
}

LongNumber operator""_longnum(long double number) {
    return LongNumber(number);
}

std::string LongNumber::toString() const {
    std::ostringstream oss;
    if (isNegative) oss << "-";
    size_t integerPartSize = digits.size() - precision;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i == integerPartSize && precision > 0) oss << ".";
        oss << digits[i];
    }
    return oss.str();
}
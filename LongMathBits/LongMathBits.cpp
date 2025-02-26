#include "LongMathBits.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <iostream>

LongNumber::LongNumber() : precision(10), isNegative(false)
{
    digits.push_back('0');
}

LongNumber::LongNumber(long double number, int precision) : precision(precision)
{
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

void LongNumber::normalize()
{
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

void LongNumber::alignPrecision(LongNumber& other)
{
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

LongNumber LongNumber::abs() const
{
    LongNumber result = *this;
    result.isNegative = false;
    return result;
}

LongNumber LongNumber::operator+(const LongNumber& other) const
{
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

LongNumber LongNumber::operator-(const LongNumber& other) const
{
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

LongNumber LongNumber::operator*(const LongNumber& other) const
{
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

    result.isNegative = (isNegative != other.isNegative);
    result.normalize();

    if (result.digits.size() >= (size_t)precision)
        result.digits.resize(result.digits.size() - precision);
    result.precision = precision;
    result.normalize();
    return result;
}



LongNumber LongNumber::operator/(const LongNumber& other) const
{
    if (other.digits.size() == 1 && other.digits[0] == '0') {
        throw std::invalid_argument("Division by 0!");
    }

    LongNumber dividend = *this;
    LongNumber divisor = other;

    int maxPrecision = std::max(dividend.precision, divisor.precision);

    LongNumber result(0, maxPrecision);

    divisor.isNegative = false;
    dividend.isNegative = false;

    int cnt = divisor.precision;
    for (int i = divisor.digits.size() - 1; i >= divisor.digits.size() - divisor.precision; --i) {
        if (divisor.digits[i] == '1') break;
        cnt--;
    }

    divisor.digits.resize(divisor.digits.size() - (divisor.precision - cnt));

    if (dividend.precision >= cnt) {
        dividend.precision -= cnt;
    } else {
        dividend.digits.insert(dividend.digits.end(), cnt - dividend.precision, '0');
        dividend.precision = 0;
    }

    int add = maxPrecision - dividend.precision;
    dividend.precision = maxPrecision;
    divisor.precision = 0;

    LongNumber temp(0, 0);

    for (int i = 0; i < (int)divisor.digits.size() - 1; ++i) {
        temp.digits.push_back(dividend.digits[i]);
    }
    temp.normalize();

    for (int i = (int)divisor.digits.size() - 1; i < (int)dividend.digits.size() + add; ++i) {
        if (temp.digits.size() > 1 || temp.digits[0] > '0') {
            temp.digits.push_back('0');
        }
        if (i < dividend.digits.size() && dividend.digits[i] == '1') {
            temp.digits[(int)temp.digits.size() - 1] = '1';
        }

        if (temp >= divisor) {
            temp = temp - divisor;
            result.digits.push_back('1');
        } else {
            result.digits.push_back('0');
        }
    }

    result.precision = dividend.precision;
    result.isNegative = (this->isNegative != other.isNegative);
    result.normalize();
    return result;
}

void alignForComparison(LongNumber &a, LongNumber &b)
{
    
    a.normalize();
    b.normalize();
    
    int commonPrecision = std::max(a.precision, b.precision);
    
    if (a.precision < commonPrecision) {
        a.digits.insert(a.digits.end(), commonPrecision - a.precision, '0');
        a.precision = commonPrecision;
    }
    if (b.precision < commonPrecision) {
        b.digits.insert(b.digits.end(), commonPrecision - b.precision, '0');
        b.precision = commonPrecision;
    }
    size_t maxSize = std::max(a.digits.size(), b.digits.size());
    while(a.digits.size() < maxSize)
        a.digits.insert(a.digits.begin(), '0');
    while(b.digits.size() < maxSize)
        b.digits.insert(b.digits.begin(), '0');
}

bool LongNumber::operator==(const LongNumber& other) const
{
    if (this->isNegative != other.isNegative)
        return false;
    
    LongNumber a = *this;
    LongNumber b = other;
    alignForComparison(a, b);
    
    return a.digits == b.digits;
}

bool LongNumber::operator!=(const LongNumber& other) const
{
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const
{
    if (this->isNegative != other.isNegative)
        return this->isNegative;
    
    LongNumber a = *this;
    LongNumber b = other;
    alignForComparison(a, b);
    
    if (!a.isNegative)
        return a.digits < b.digits;
    else
        return a.digits > b.digits;
}

bool LongNumber::operator>(const LongNumber& other) const
{
    return other < *this;
}

bool LongNumber::operator<=(const LongNumber& other) const
{
    return !(*this > other);
}

bool LongNumber::operator>=(const LongNumber& other) const
{
    return !(*this < other);
}


void LongNumber::setPrecision(int newPrecision)
{
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

LongNumber operator""_longnum(long double number)
{
    return LongNumber(number);
}

std::string LongNumber::MultStringByTwo(const std::string &s) const
{
    std::string result = "";
    int carry = 0;
    for (int i = (int)s.size() - 1; i >= 0; --i)
    {
        int digit = s[i] - '0';
        int value = digit * 2 + carry;
        carry = value / 10;
        result.push_back(char(value % 10 + '0'));
    }

    result.push_back(char('0' + carry));

    std::reverse(result.begin(), result.end());
    return result;
}

std::string LongNumber::DivStringByTwo(const std::string &s) const
{
    std::string result = "";
    int carry = 0;
    for (char c : s)
    {
        int digit = c - '0';
        int value = carry * 10 + digit;
        carry = value % 2;
        result.push_back(char('0' + value / 2));
    }
    while (carry != 0)
    {
        carry *= 10;
        result.push_back(char('0' + carry / 2));
        carry %= 2;
    }
    return result;
}

std::string LongNumber::SumOfTwoStrings(const std::string &num1, const std::string &num2, int type) const
{
    int len = std::max(num1.size(), num2.size());
    std::string a(num1), b(num2);

    if (type == 0)
    {
        a.insert(a.begin(), len - num1.size(), '0');
        b.insert(b.begin(), len - num2.size(), '0');
    }
    else
    {
        a.insert(a.end(), len - num1.size(), '0');
        b.insert(b.end(), len - num2.size(), '0');
    }

    std::string result = "";
    int carry = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        int digit1 = a[i] - '0';
        int digit2 = b[i] - '0';
        int value = digit1 + digit2 + carry;
        carry = value / 10;
        value %= 10;
        result.push_back(char('0' + value));
    }
    if (type == 0)
        result.push_back(char('0' + carry));
    std::reverse(result.begin(), result.end());
    return result;
}

void LongNumber::MakeStringNorm(std::string & s, int type) const{
    if(type == 0){
        while(s[0] == '0')
            s.erase(s.begin());
    }
    else{
        while(s.back() == '0')
            s.pop_back();
    }
}

std::string LongNumber::toString() const
{
    std::string integerPart = "", fractionalPart = "";
    std::string tmp = "1";

    for (int i = (int)digits.size() - precision - 1; i >= 0; --i)
    {
        if (digits[i] == '1')
            integerPart = SumOfTwoStrings(integerPart, tmp, 0);
        
            tmp = MultStringByTwo(tmp);

        MakeStringNorm(tmp, 0);
        MakeStringNorm(integerPart, 0);
    }

    tmp = "5";
    for (int i = (int)digits.size() - precision; i < (int)digits.size(); ++i)
    {
        if (digits[i] == '1')
            fractionalPart = SumOfTwoStrings(fractionalPart, tmp, 1);

            tmp = DivStringByTwo(tmp);

        
            MakeStringNorm(tmp, 1);
            MakeStringNorm(fractionalPart, 1);
    }
        MakeStringNorm(fractionalPart, 1);
        MakeStringNorm(integerPart, 0);

    if (integerPart.empty())
        integerPart = "0";
    if (fractionalPart.empty())
        fractionalPart = "0";

    if(isNegative)
        integerPart = "-" + integerPart;

    return integerPart + "." + fractionalPart;
}
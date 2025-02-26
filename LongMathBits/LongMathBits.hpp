#ifndef LONGMATH_HPP
#define LONGMATH_HPP

#include <string>
#include <vector>

class LongNumber {
public:
    LongNumber();
    LongNumber(long double number, int precision = 10);
    LongNumber(const LongNumber& other);
    ~LongNumber();

    LongNumber& operator=(const LongNumber& other);

    LongNumber operator+(const LongNumber& other) const;
    LongNumber operator-(const LongNumber& other) const;
    LongNumber operator*(const LongNumber& other) const;
    LongNumber operator/(const LongNumber& other) const;

    bool operator==(const LongNumber& other) const;
    bool operator!=(const LongNumber& other) const;
    bool operator<(const LongNumber& other) const;
    bool operator>(const LongNumber& other) const;
    bool operator>=(const LongNumber& other) const;
    bool operator<=(const LongNumber& other) const;

    void setPrecision(int newPrecision);

    std::string toString() const;

private:
    std::vector<char> digits; 
    int precision;            
    bool isNegative;   
    void normalize();
    void alignPrecision(LongNumber& other);
    friend void alignForComparison(LongNumber &a, LongNumber &b);
    LongNumber abs() const;
    std::string SumOfTwoStrings(const std::string &num1, const std::string &num2, int type) const;
    std::string DivStringByTwo(const std::string &s) const;
    std::string MultStringByTwo(const std::string &s) const;
    void MakeStringNorm(std::string & s, int type) const;
};

LongNumber operator""_longnum(long double number);

#endif // LONGMATH_HPP
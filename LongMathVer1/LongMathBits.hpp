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

    void setPrecision(int newPrecision);

    friend LongNumber operator""_longnum(long double number);

    std::string toString() const;

private:
    std::vector<char> digits; 
    int precision;            
    bool isNegative;   
    void normalize();
    void alignPrecision(LongNumber& other);
    LongNumber abs() const;
};

#endif // LONGMATH_HPP
#ifndef LONGMATH_HPP
#define LONGMATH_HPP

#include <string>
#include <vector>

class LongNumber
{
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
        std::vector<bool> integerPart; 
        std::vector<bool> fractionalPart; 
        int precision; 
        bool isNegative; 

        void normalize(); 
        void alignPrecision(LongNumber& other); 
        void toBinary(long double number); 
        long double toDouble() const; 
        void shiftLeft(size_t count = 1); 
        void shiftRight(size_t count = 1); 
};

#endif // LONGMATH_HPP
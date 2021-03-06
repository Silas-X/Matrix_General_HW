#include "fraction.h"
#include <cmath>
#include <iostream>

int Gcd(const int& a, const int& b) { return (b ? Gcd(b, a % b) : a); }
long long Gcd(const long long& a, const long long& b) {
  return (b ? Gcd(b, a % b) : a);
}
int fraction::gcd(const int& a, const int& b) const {
  //  return (b ? gcd(b, a % b) : a);
  return Gcd(a, b);
}
int lcm(const int& a, const int& b) {
  int gcd = Gcd(a, b);
  return (a * b / gcd);
}
long long lcm(const long long& a, const long long& b) {
  long long gcd = Gcd(a, b);
  return (a * b / gcd);
}
void fraction::simp() {
  // exception: An illegal fraction expression.
  if (_denominator == 0) {
    _numerator = 0;
    return;
  }

  // The regular simplification process.
  int sign=1;
  if (((this->_numerator < 0) && (this->_denominator > 0)) ||
      ((this->_numerator > 0) && (this->_denominator < 0)))
    sign = -1;
  if (sign < 0) {
    this->_numerator = -abs(this->_numerator);
    this->_denominator = abs(this->_denominator);
  } else {
    this->_numerator = abs(this->_numerator);
    this->_denominator = abs(this->_denominator);
  }
  int factor = gcd(abs(this->_numerator), abs(this->_denominator));
  this->_denominator /= factor;
  this->_numerator /= factor;  //分数化简
}

fraction::fraction(const int& numerator, const int& denominator)
    : _numerator{numerator}, _denominator{denominator} {
  // Should We Simplify it?Or let it be?
  simp();
}

fraction::fraction(const fraction& other) {
  this->_denominator = other._denominator;
  this->_numerator = other._numerator;
  // Should We Simplify it?Or let it be?
  simp();
}

void fraction::operator=(const fraction& other) {
  this->_denominator = other._denominator;
  this->_numerator = other._numerator;
}

fraction fraction::operator+(const fraction& other) const {
  // exception either denominator is 0, return 0/0;
  if (!this->_denominator || !other._denominator) return fraction{0, 0};

  // regular addition for fractions.
  // 0 can be processed as well, don't need to special operation.
  fraction temp{0, 0};
  /*
   *temp._denominator = lcm(this->_denominator, other._denominator);
   *temp._numerator =
   *     this->_numerator * (temp._denominator / this->_denominator) +
   *      other._numerator * (temp._denominator / other._denominator);
   */
  // Long long buffer version
  long long denominator =
      lcm((long long)this->_denominator, (long long)(other._denominator));
  long long numerator = this->_numerator * (denominator / this->_denominator) +
                        other._numerator * (denominator / other._denominator);
  long long factor = Gcd(numerator, denominator);
  numerator /= factor;
  denominator /= factor;
  temp._denominator = denominator;
  temp._numerator = numerator;
  temp.simp();
  return temp;
}

fraction fraction::operator-(const fraction& other) const {
  /*
   * exception
   *if (!this->_denominator && !other._denominator) return fraction{0, 0};
   * regular substraction for fractions.
   * 0 can be processed as well, don't need to special operation.
   * Judeged by operator+;
   */
  fraction n_other{-other._numerator, other._denominator};
  return (*this + n_other);
}

fraction fraction::operator*(const fraction& other) const {
  // exception
  if (!this->_denominator || !other._denominator) return fraction{0, 0};
  // regular operation multiplication

  // Crossing simplify
  fraction frac1{*this}, frac2{other};
  // frac1.simp();
  // frac2.simp();
  {
    int temp;
    temp = frac1._numerator;
    frac1._numerator = frac2._numerator;
    frac2._numerator = temp;
    frac1.simp();
    frac2.simp();
  }

  // Main multiplication
  fraction temp;
  temp._denominator = frac1._denominator * frac2._denominator;
  temp._numerator = frac1._numerator * frac2._numerator;
  temp.simp();  // maybe a useless step;
  return temp;
}

fraction fraction::operator/(const fraction& other) const
// relay on operator *
{
  // exception judeged by *
  fraction reciprocal{other._denominator, other._numerator};
  return *this * reciprocal;
}

void fraction::operator+=(const fraction& other) {
  fraction temp = *this + other;
  *this = temp;
}

void fraction::operator-=(const fraction& other) {
  fraction temp = *this - other;
  *this = temp;
}

void fraction::operator*=(const fraction& other) {
  fraction temp = *this * other;
  *this = temp;
}

void fraction::operator/=(const fraction& other) {
  fraction temp = *this / other;
  *this = temp;
}

bool fraction::operator==(const fraction& other) const {
  // exception both are illegal;
  if (!this->_denominator && !other._denominator) return true;
  if ((!this->_denominator && other._denominator) ||
      (this->_denominator && !other._denominator))
    return false;
  // regular equality;
  return ((*this - other)._numerator == 0);
}

bool fraction::operator!=(const fraction& other) const {
  return !(*this == other);
}

bool fraction::operator<(const fraction& other) const {
  return ((*this - other)._numerator < 0);
}

bool fraction::operator>(const fraction& other) const {
  return ((*this - other)._numerator > 0);
}

bool fraction::operator<=(const fraction& other) const {
  return !(*this > other);
}

bool fraction::operator>=(const fraction& other) const {
  return !(*this < other);
}

std::istream& operator>>(std::istream& in, fraction& frac)
// Input Format: two integers with a space in it
// "a b" is correct. Not "a/b"
{
  int a, b;
  in >> a >> b;
  frac._numerator = a;
  frac._denominator = b;
  return in;
}

std::ostream& operator<<(std::ostream& out, const fraction& frac)
// Normally you should output "a/b" without any space and LF
// Sometimes you may output a single integer (Why? Guess XD)
// If it is not a number (den = 0), output "NaN"

{
  if (frac._denominator == 0) {
    out << "NaN";
    return out;
  }
  if (frac._denominator == 1) {
    out << frac._numerator;
    return out;
  }
  out << frac._numerator << '/' << frac._denominator;
  return out;
}
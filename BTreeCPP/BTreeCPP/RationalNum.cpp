#include "RationalNum.h"


RationalNum::RationalNum() : a(0), b(1) {}

RationalNum::RationalNum(int A) : a(A), b(1) {
	simplify();
}
RationalNum::RationalNum(int A, int B) : a(A), b(B) {
	simplify();
}


int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

int RationalNum::getA()
{
	return a;
}

void RationalNum::simplify() {
	int GCD = gcd(a, b);
	a /= GCD;
	b /= GCD;
	if ((b < 0 && a < 0) || b < 0)
	{
		b *= -1;
		a *= -1;
	}
}

RationalNum RationalNum::operator+(const RationalNum& other)
{
	int newA = a * other.b + b * other.a;
	int newB = b * other.b;
	RationalNum result(newA, newB);
	result.simplify();
	return result;
}

RationalNum RationalNum::operator-(const RationalNum& other)
{
	int newA = a * other.b - b * other.a;
	int newB = b * other.b;
	RationalNum result(newA, newB);
	result.simplify();
	return result;
}

RationalNum RationalNum::operator*(const RationalNum& other)
{
	int newA = a * other.a;
	int newB = b * other.b;
	RationalNum result(newA, newB);
	result.simplify();
	return result;
}

RationalNum RationalNum::operator/(const RationalNum& other)
{
	if (other.a == 0)
	{
		std::cerr << "Division by zero is not allowed." << std::endl;
		return RationalNum(0, 1);
	}
	int newA = a * other.b;
	int newB = b * other.a;
	RationalNum result(newA, newB);
	result.simplify();
	return result;
}

RationalNum& RationalNum::operator+=(const RationalNum& other) {
	int newA = a * other.b + b * other.a;
	int newB = b * other.b;
	a = newA;
	b = newB;
	simplify();
	return *this;
}

RationalNum& RationalNum::operator-=(const RationalNum& other) {
	int newA = a * other.b - b * other.a;
	int newB = b * other.b;
	a = newA;
	b = newB;
	simplify();
	return *this;
}

RationalNum& RationalNum::operator*=(const RationalNum& other) {
	int newA = a * other.a;
	int newB = b * other.b;
	a = newA;
	b = newB;
	simplify();
	return *this;
}

RationalNum& RationalNum::operator/=(const RationalNum& other) {
	if (other.a == 0) {
		std::cerr << "Division by zero is not allowed." << std::endl;
		return *this;
	}
	int newA = a * other.b;
	int newB = b * other.a;
	a = newA;
	b = newB;
	simplify();
	return *this;
}

bool RationalNum::operator==(const RationalNum& other) {
	return a == other.a && b == other.b;
}

std::ostream& operator<<(std::ostream& os, const RationalNum& num) {
	if (num.b == 1)
	{
		os << num.a;
	}
	else
	{
		os << num.a << "/" << num.b;
	}

	return os;
}
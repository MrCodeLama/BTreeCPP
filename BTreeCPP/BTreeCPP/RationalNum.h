#pragma once
#include <ostream>
#include <iostream>

using namespace std;

class RationalNum
{
private:
	int a, b;
public:

	RationalNum();
	RationalNum(int A, int B);
	RationalNum(int A);

	int getA();
	void simplify();
	RationalNum operator+(const RationalNum& other);
	RationalNum operator-(const RationalNum& other);
	RationalNum operator*(const RationalNum& other);
	RationalNum operator/(const RationalNum& other);
	RationalNum& operator+=(const RationalNum& other);
	RationalNum& operator-=(const RationalNum& other);
	RationalNum& operator*=(const RationalNum& other);
	RationalNum& operator/=(const RationalNum& other);
	bool operator==(const RationalNum& other);
	friend std::ostream& operator<<(std::ostream& os, const RationalNum& num);
};

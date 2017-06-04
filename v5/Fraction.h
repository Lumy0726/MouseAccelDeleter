#ifndef FRACTION_HED
#define FRACTION_HED

class C_fraction{
protected:
	int Numerator;
	unsigned Denominator;
	//조건문 시간차이 없에기 위함.
	volatile static int tempnum1;
	volatile static int tempnum_1;
	volatile static int tempnum2;
	volatile static int tempnum_2;

	void Reduce();

public:
	C_fraction(int Input = 0);
	C_fraction(int Numer, unsigned Denom);

	const C_fraction operator+(const C_fraction& Input) const;
	const C_fraction operator-(const C_fraction& Input) const;
	const C_fraction operator*(const C_fraction& Input) const;
	const C_fraction operator/(const C_fraction& Input) const;

	bool operator>(const C_fraction& Input) const;
	bool operator<(const C_fraction& Input) const;
	bool operator>=(const C_fraction& Input) const;
	bool operator<=(const C_fraction& Input) const;
	bool operator==(const C_fraction& Input) const;
	bool operator!=(const C_fraction& Input) const;

	int Get_numer() const;
	unsigned Get_denom() const;
	double Get_value() const;
	int Get_value_int() const;
	static unsigned Get_divisor(unsigned a, unsigned b);
	static int absINT(int input);
};
#endif
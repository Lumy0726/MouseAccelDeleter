#include "Fraction.h"
#include <assert.h>

//조건문 시간차이 없에기 위함.
volatile int C_fraction::tempnum1 = 1;
volatile int C_fraction::tempnum_1 = -1;
volatile int C_fraction::tempnum2 = 2;
volatile int C_fraction::tempnum_2 = -2;

//function define
C_fraction::C_fraction(int Input){
	Numerator = Input;
	Denominator = 1;
}
C_fraction::C_fraction(int Numer, unsigned Denom){
	Numerator = Numer;
	Denominator = Denom;
	if (Denom == 0){
		assert(("Division with 0.",false));
		Numerator = 0;
		Denominator = 1;
	}
	Reduce();
}
void C_fraction::Reduce(){
	unsigned Divisor = Get_divisor(unsigned(absINT(Numerator)), Denominator);
	Numerator /= (int)Divisor;
	Denominator /= Divisor;
}
const C_fraction C_fraction::operator+(const C_fraction& Input) const{
	return C_fraction(Numerator*int(Input.Denominator) + (Input.Numerator)*int(Denominator), Denominator*(Input.Denominator));
}
const C_fraction C_fraction::operator-(const C_fraction& Input) const{
	return C_fraction(Numerator*int(Input.Denominator) - (Input.Numerator)*int(Denominator), Denominator*(Input.Denominator));
}
const C_fraction C_fraction::operator*(const C_fraction& Input) const{
	return C_fraction(Numerator*(Input.Numerator), Denominator*(Input.Denominator));
}
const C_fraction C_fraction::operator/(const C_fraction& Input) const{
	if (Input.Numerator){
		if (Input.Numerator > 0){
			return C_fraction(tempnum1 * Numerator*int(Input.Denominator), unsigned(tempnum1 * int(Denominator)*(Input.Numerator)));
		}
		else {
			return C_fraction(tempnum_1 * Numerator*int(Input.Denominator), unsigned(tempnum_1 * int(Denominator)*(Input.Numerator)));
		}
	}
	else {
		assert(("Division with 0.", false));
		return 0;
	}
}
bool C_fraction::operator>(const C_fraction& Input) const{
	return Numerator * int(Input.Denominator) > Input.Numerator * int(Denominator);
}
bool C_fraction::operator<(const C_fraction& Input) const{
	return !(*this>=Input);
}
bool C_fraction::operator>=(const C_fraction& Input) const{
	return Numerator * int(Input.Denominator) >= Input.Numerator * int(Denominator);
}
bool C_fraction::operator<=(const C_fraction& Input) const{
	return !(*this > Input);
}
bool C_fraction::operator==(const C_fraction& Input) const{
	return (Numerator == Input.Numerator) && (Denominator == Input.Denominator);
}
bool C_fraction::operator!=(const C_fraction& Input) const{
	return !(*this == Input);
}
int C_fraction::Get_numer() const{
	return Numerator;
}
unsigned C_fraction::Get_denom() const{
	return Denominator;
}
double C_fraction::Get_value() const{
	if (Denominator){
	return ((double)Numerator) / Denominator;
	}
	else {
		assert(("Division with 0.", false)); return 0;
	}
}
int C_fraction::Get_value_int() const {
	if (Denominator) {
		return Numerator / int(Denominator);
	}
	else {
		assert(("Division with 0.", false)); return 0;
	}
}
unsigned C_fraction::Get_divisor(unsigned a, unsigned b){
	if (b == 0){
		assert(("Division with 0.", false)); return 1;
	}
	if (a == 0){
		return b;
	}
	else if (((a>b) ? a : b) % ((a>b) ? b : a) == 0){
		return ((a>b) ? b : a);
	}
	else {
		return Get_divisor(((a>b) ? b : a), ((a>b) ? a : b) % ((a>b) ? b : a));
	}
}
int C_fraction::absINT(int input) {
	if (input > 0) {
		return input * tempnum1;//absINT의 리턴 시간을 최대한 동일하게 하기 위한 절차.
	}
	else {
		return input * tempnum_1;
	}
}
//verge

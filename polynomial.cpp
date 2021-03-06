#include "polynomial.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cfloat>

using std::istream;
using std::ostream;
using std::string;
using std::stringstream;
using std::fixed;
using std::setprecision;
using std::showpos;

Polynomial::Polynomial(size_t degree) : _degree(degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = 0.0;
	}
}
Polynomial::Polynomial(size_t degree, const float* coefficients): _degree(degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = coefficients[i];
	}
}
Polynomial::Polynomial(const Polynomial& polynomial): _degree(polynomial._degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = polynomial._coefficients[i];
	}
}
Polynomial::~Polynomial(){
	//only coefficients need to be removed from the heap in order to prevent memory leak
	delete[] _coefficients;
}
const Polynomial Polynomial::Sum(const Polynomial& rhs)const {
	//when summing polynomials, we will first check which one has higher order
	if (_degree > rhs._degree)
	{
		//if it is the one to which "this" pointer points
		//create clone of it (since this function is const, we cannot change this object)
		//this will be return value from the function
		Polynomial retVal(*this);
		//coefficients that are not present in rhs._coefficients will remain the same
		//only the ones which are present in both polynomials will be summed
		for (size_t i = 0; i < rhs._degree + 1; i++)
		{
			//go through all of them and add them to corresponding coefficients from retVal 
			retVal._coefficients[i] += rhs._coefficients[i];
		}
		//at the end, return retVal
		return retVal;
	}
	else
	{
		//if we are here, this means that rhs polynomial has more coefficients compared
		//to this object (rhs is higher order polynomial). So, repeat all from above, 
		//but with swapped roles. First, create polynomial (which will be a return value)
		//base on rhs polynomial of a higher order
		Polynomial retVal(rhs);
		//go through all coefficients from this object
		//just to make it more clear, you can refer to _degree also as this->_degree, 
		//and to _coefficients as this->_coefficients
		for (size_t i = 0; i < _degree + 1; i++)
		{
			//add overlaping coefficients from this object to the ones from retVal
			retVal._coefficients[i] += _coefficients[i];
		}
		//return retVal once you are done
		return retVal;
	}
}

const Polynomial Polynomial::Subtract(const Polynomial& rhs)const{
	//create clone of this object which will be return value
	//from a function (hence retVal, similar as above)
	Polynomial retVal(*this);
	//subtract of polynomial can be performed 
	//using sum and minus on the other (P1 - P2 = P1 + (-P2))
	return retVal.Sum(rhs.Minus());
	
}

const Polynomial Polynomial::Minus()const{
	Polynomial retVal(*this);
	for (size_t i = 0; i < _degree + 1; i++) {
		retVal._coefficients[i] *= -1;
	}
	return retVal;
}

const Polynomial Polynomial::Multiply(const Polynomial& rhs)const{
	//when multiplying polynomials, degree of a product will be the sum of both
	int degree = _degree + rhs._degree;
	//create new polynomial of a given degree (please note that all coefficients will be zero
	//as a result of a constructor from above (the first one given). This will be return value
	Polynomial retVal(degree);
	//loop through all coefficients from the first polynomial
	for (size_t i = 0; i < _degree + 1; i++) {
		//and loop through all coefficients from the other polynomial
		for (size_t j = 0; j < rhs._degree + 1; j++) {
			//multiply coefficients at positions i and j and store them at 
			//position i+j in the resulting polynomial (for example, if you
			//multiply 2x^2 and 3x^3 result will be 6x^5 which is 2*3 stored 
			//at position 5 in the resulting polynomial
			retVal._coefficients[i + j] += _coefficients[i] * rhs._coefficients[j];
		}
	}
	return retVal;
	
}
const Polynomial Polynomial::Divide(const Polynomial& rhs)const{
	return Polynomial(0);
}
const Polynomial Polynomial::Derive()const{
	//when doing derivation, degree of a resulting polynomial is always decreased by 1
	//create new polynomial with all zero coefficients and degree = _degree - 1
	//this will be a return value of a function
	Polynomial retVal(_degree - 1);
	//iterate through all of the coefficients, multiply position i (starting from 1) with 
	//coefficient at that position and store the result at position i-1 in the resulting 
	//polynomial. For example, when 3x^2 is derived, result is 3*2x (position of original
	//coefficient was 2 because of x^2, coefficient was 3, so 3*2 will be stored at the
	//position 1 (x) in the resulting polynomial. Repeat the same for all coefficients from
	//the starting polynomial
	for (size_t i = 0; i < _degree; i++) {
		retVal._coefficients[i] = _coefficients[i + 1] * (i + 1);
	}
	//return the result
	return retVal;
	
}
float Polynomial::Evaluate(float x)const{
	//result that will be return once we are done
	float result = 0;
	//term will hold exponent values while we go through the coefficients
	//first it is 1, then it will be x, then x^2, ....
	float term = 1;
	//iterate through all of the coefficients
	for (size_t i = 0; i < _degree + 1; i++) {
		//update previous result with new value
		//in first iteration it will be result = coefficient[0] * 1
		//in second iteration we will add coefficient[1]*1*x to previously calculated
		//result, so the result will be coefficient[0] + coefficient[1]*x
		//... and in that manner until we iterate through all coefficients	
		result += term * _coefficients[i];
		//update term variable, so it is ready for the next iteration loop (first 1, then x, then x^2,...)
		term *= x;
	}

	return result;
	
}
float Polynomial::Integrate(float start, float end)const{
	//when doing integration, degree of a resulting polynomial is always increased by 1
	//create new polynomial with all zero coefficients and degree = _degree + 1
	//this will be a return value of a function
	Polynomial retVal(_degree + 1);
	//iterate through all of the coefficients, first position will be zero, second will be
	//coefficient[0] from starting polynomial, and for the
	//rest divide coefficient at position i (starting from 1) with position (i+1), and 
	//store the result at position i+1 in the resulting 
	//polynomial. For example, when 3x^2 is integrated, result is 3/3*x^3 (position of original
	//coefficient was 2 because of x^2, coefficient was 3, so 3/(2+1) will be stored at the
	//position 3 (x^3) in the resulting polynomial. Repeat the same for all coefficients from
	//the starting polynomial
	retVal._coefficients[0] = 0;
	retVal._coefficients[1] = _coefficients[0];
	for (size_t i = 1; i < _degree + 1; i++) {
		retVal._coefficients[i + 1] = _coefficients[i] / (i + 1);
	}
	//now, we have integrated the polynomial, in order to calculate integral over interval we need to
	//evaluate integral at end (argument) and subtract from it evaluated integral at start(argument)
	return retVal.Evaluate(end) - retVal.Evaluate(start);
	
}
const Polynomial& Polynomial::operator=(const Polynomial& rhs){
	if (&rhs == this){
		return *this;
	}
	if (_degree != rhs._degree){
		if (_coefficients){
			delete[] _coefficients;
		}
		_degree = rhs._degree;
		_coefficients = new float[_degree + 1];
	}
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = rhs._coefficients[i];
	}
	return *this;
}
bool Polynomial::Equals(const Polynomial& rhs)const{
	if (_degree != rhs._degree){
		return false;
	}
	for (size_t i=0; i < _degree; i++){
		if (abs(_coefficients[i] - rhs._coefficients[i]) > 0.0001){
			return false;
		}
	}
	return true;
}
string Polynomial::ToString()const{
	stringstream ss;
	for (size_t i = _degree; i > 0; i--) {
		ss << showpos << fixed << setprecision(2) << _coefficients[i] << "x^" << i << " ";
	}
	ss << showpos << fixed << setprecision(2) << _coefficients[0];
	return ss.str();
}
ostream& Polynomial::Write(ostream& output)const{
	output << _degree << " ";
	for (size_t i = 0; i < _degree + 1; i++) {
		output << _coefficients[i] << " ";
	}
	return output;
}
istream& Polynomial::Read(istream& input){
	size_t degree;
	input >> degree;
	if (input.fail()){
		return input;
	}
	float* coefficients = new float[degree + 1];
	for (size_t i = 0; i < degree + 1; i++) {
		input >> coefficients[i];
		if (input.fail()){
			delete[] coefficients;
			return input;
		}
	}

	if (degree != _degree){
		if (_coefficients){
			delete[] _coefficients;
		}
		_degree = degree;
		_coefficients = coefficients;
	}else{
		for (size_t i = 0; i < _degree + 1; i++) {
			_coefficients[i] = coefficients[i];
		}
		delete[] coefficients;
	}
	return input;
}

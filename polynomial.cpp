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
	return Polynomial(0);
}
const Polynomial Polynomial::Minus()const{
	Polynomial retVal(*this);
	for (size_t i = 0; i < _degree + 1; i++) {
		retVal._coefficients[i] *= -1;
	}
	return retVal;
}
const Polynomial Polynomial::Multiply(const Polynomial& rhs)const{
	return Polynomial(0);
}
const Polynomial Polynomial::Divide(const Polynomial& rhs)const{
	return Polynomial(0);
}
const Polynomial Polynomial::Derive()const{
	return Polynomial(0);
}
float Polynomial::Evaluate(float x)const{
	return FLT_MAX;
}
float Polynomial::Integrate(float start, float end)const{
	return FLT_MAX;
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

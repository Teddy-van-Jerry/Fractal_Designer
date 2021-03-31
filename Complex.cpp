/*
 * Project: Fractal Designer
 * File: Complex.cpp
 * Platform: Qt 6.0.0
 * -----------------
 * This file implements the Complex.h interface.
 *
 * @author Wuqiong Zhao, Junhao Wang, Pengfei Xie, Yaobing Zheng
 *
 * @version 2021/03/31
 * - simplify the return statement
 * - add function setReal and setImaginary
 * - add inline
 * @version 2020/12/29
 * - modift to fit Qt
 * @version 2020/12/28
 * - add the operator =
 * - add the function sqrt
 * - solve bugs which could lead to vector subscript out of range with illegal input
 * @version 2020/12/27
 * - use the template to deal with operators that take different parameter types
 * - move operator +, -, *, / from the Complex class
 * - add power function that takes a Complex
 * - add log function
 * @version 2020/12/22
 * - add two overload operator/ which take double and int
 * - add the exp, sin, cos function
 * - add the unary plus and the unary minus
 * @version 2020/12/13
 * - modify the accuracy from 1E-16 to 1E-12
 * - add the error type "No input"
 * - modify the power function which takes a double
 * - add the argz function
 * - delete the power function which takes a int
 * - add the function complex_is and complex_is_information
 * - move functions in Calculator.cpp here
 * @version 2020/12/10
 * - modify some parameters into const reference
 * @version 2020/11/29
 * - add the convertion function(operator double())
 * @version 2020/11/27
 * - add function +=, -=, *=, /=, ^=
 * - add function error
 * - add function divide and power
 * - add function minus and multiply
 * - add function conjugate and modulus
 * @version 2020/11/26
 * - initial version
 */

#include "Complex.h"

Complex::Complex(double a, double b): Real(a), Imaginary(b) {}

void Complex::setReal(const double& real_)
{
    this->Real = real_;
}

void Complex::setImaginary(const double& imag_)
{
    this->Imaginary = imag_;
}

Complex Complex::add(const Complex& a) const
{
    // *this refers to the initial Complex number
    // (*this).func() equals to this->func()
    // (*this).member equals to this->member
    double new_Real = this->Real + a.Real;
    double new_Imaginary = this->Imaginary + a.Imaginary;
    return Complex(new_Real, new_Imaginary);
}

Complex Complex::minus(const Complex& a) const
{
    double new_Real = this->Real - a.Real;
    double new_Imaginary = this->Imaginary - a.Imaginary;
    return Complex(new_Real, new_Imaginary);
}

Complex Complex::multiply(const Complex& a) const
{
    double new_Real = this->Real * a.Real - this->Imaginary * a.Imaginary;
    double new_Imaginary = this->Real * a.Imaginary + a.Real * this->Imaginary;
    return Complex(new_Real, new_Imaginary);
}

Complex Complex::divide(const Complex& a) const
{
    double A = this->Real;
    double B = this->Imaginary;
    double C = a.Real;
    double D = a.Imaginary;
    if (C == 0 && D == 0)
    {
        //error("Can't be divided by zero!");
        //Complex result(Inf, Inf);
        return 0;
    }
    else
    {
        double new_Real = (A * C + B * D) / (C * C + D * D);
        double new_Imaginary = (C * B - A * D) / (C * C + D * D);
        return Complex(new_Real, new_Imaginary);
    }
}

Complex Complex::power(const double& n) const
{
    // De Moivre's formula
    double Argz = n * this->argz();
    double r = std::pow(this->modulus().Real, n);
    return Complex(r * cos(Argz), r * sin(Argz));
}

Complex Complex::power(const Complex& a) const
{
    if (a.Imaginary == 0)
    {
        return this->power(a.Real);
    }
    else
    {
        return exp(log(*this) * a);
    }
}

Complex Complex::sqrt(const Complex& a) const
{
    if (a.Real == 0 && a.Imaginary == 0)
    {
        //        error("Mathematic error!\nRoot sign cannot be zero!");
        return 0;
    }
    else
    {
        return this->power(1 / a);
    }
}

Complex Complex::conjugate() const
{
    double new_Real = this->Real;
    double new_Imaginary = -this->Imaginary;
    return Complex(new_Real, new_Imaginary);
}

Complex Complex::modulus() const
{
    double mod = std::sqrt(pow(this->Real, 2) + pow(this->Imaginary, 2));
    return Complex(mod, 0);
}

 double Complex::getReal() const
{
    return this->Real;
}

 double Complex::getImaginary() const
{
    return this->Imaginary;
}

 double Complex::argz() const
{
    return atan2(this->Imaginary, this->Real);
}

Complex Complex::operator-()
{
    return Complex(-this->Real, -this->Imaginary);
}

Complex Complex::operator+()
{
    return *this;
}

Complex Complex::operator+=(const Complex& a)
{
    *this = this->add(a);
    return *this;
}

Complex Complex::operator-=(const Complex& a)
{
    *this = this->minus(a);
    return *this;
}

Complex Complex::operator*=(const Complex& a)
{
    *this = this->multiply(a);
    return *this;
}

Complex Complex::operator/=(const Complex& a)
{
    *this = this->divide(a);
    return *this;
}

Complex Complex::operator^(const double& n)
{
    return this->power(n);
}

Complex Complex::operator^=(const double& n)
{
    *this = this->power(n);
    return *this;
}

Complex Complex::operator^(const Complex& n)
{
    return this->power(n);
}

Complex Complex::operator^=(const Complex& n)
{
    *this = this->power(n);
    return *this;
}

bool Complex::operator==(const Complex& a)
{
    if (this->Real == a.Real && this->Imaginary == a.Imaginary)
        return true;
    else return false;
}

bool Complex::operator!=(const Complex& a)
{
    if (*this == a) return false;
    else return true;
}

void Complex::operator=(const Complex& a)
{
    this->Real = a.Real;
    this->Imaginary = a.Imaginary;
}

template<typename T_>
void Complex::operator=(const T_& a)
{
    *this = static_cast<Complex>(a);
}

Complex::operator double()
{
    return Real;
}

Complex operator+(const Complex& a, const Complex& b)
{
    return a.add(b);
}

Complex operator-(const Complex& a, const Complex& b)
{
    return static_cast<Complex>(a).minus(static_cast<Complex>(b));
}

Complex operator*(const Complex& a, const Complex& b)
{
    return static_cast<Complex>(a).multiply(static_cast<Complex>(b));
}

Complex operator/(const Complex& a, const Complex& b)
{
    return static_cast<Complex>(a).divide(static_cast<Complex>(b));
}

Complex exp(const Complex& n)
{
    //Complex i(0, 1);
    Complex ret = std::exp(n.getReal());
    Complex i_ret(cos(n.getImaginary()), sin(n.getImaginary()));
    ret *= i_ret;
    return ret;
}

Complex log(const Complex& n)
{
    return Complex(log(static_cast<double>(n.modulus())), n.argz());
}

Complex cos(const Complex& a)
{
    Complex i(0, 1);
    return (exp(i * a) + exp(-i * a)) / static_cast<Complex>(2);
}

Complex sin(const Complex& a)
{
    Complex i(0, 1);
    return (exp(i * a) - exp(-i * a)) / (static_cast<Complex>(2) * i);
}



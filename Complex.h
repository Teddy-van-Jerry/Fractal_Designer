/*
 * Project: Fractal Designer
 * File: Complex.h
 * Platform: Qt 6.0.0
 * -----------------
 *
 * @author Teddy van Jerry
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
 * - add the constrcutor list that takes a string
 * - modify the accuracy from 1E-16 to 1E-12
 * - add the error type "No input"
 * - modify the power function which takes a double
 * - add the argz function
 * - delete the power function which takes a int
 * - add the function complex_is and complex_is_information
 * - move functions in Calculator.h here
 * @version 2020/12/10
 * - modify some parameters into const reference
 * @version 2020/11/29
 * - add the conversion function(operator double())
 * @version 2020/11/27
 * - add function +=, -=, *=, /=, ^=
 * - add function error
 * - add function divide and power
 * - add function minus and multiply
 * - add function conjugate and modulus
 * @version 2020/11/26
 * - initial version
 */

#ifndef _COMPLEX_
#define _COMPLEX_
#include <cmath>

//Complex complex_is(std::string line); // declare

class Complex {

public:

    // function prototype(declaration)

    /**
     * Constructor list
     * used to initialize members Real and Inaginary
     * @ written by Teddy van Jerry
     */
    Complex(double a = 0, double b = 0);

    /**
     * SetReal function
     * return void
     * @ written by Teddy van Jerry
     */
    void setReal(const double& real_);

    /**
     * SetImaginary function
     * return void
     * @ written by Teddy van Jerry
     */
    void setImaginary(const double& imag_);

    /**
     * Add function
     * used to add two Complex numbers
     * return the sum of the two numbers
     * @ written by Teddy van Jerry
     */
    Complex add(const Complex& a) const;

    /**
     * Minus function
     * used to minus two Complex numbers
     * return the difference of the two numbers
     * @ written by WJH
     */
    Complex minus(const Complex& a) const;

    /**
     * Multiply function
     * used to multiply two Complex numbers
     * return the difference of the two numbers
     * @ written by WJH
     */
    Complex multiply(const Complex& a) const;

    /**
     * Divide function
     * used to divide two Complex numbers
     * return the product of the two numbers
     * @ written by ZYB
     */
    Complex divide(const Complex& a) const;

    /**
     * Power function(double)
     * used to power one Complex number
     * return the power of it
     * @ written by Teddy van Jerry
     */
    Complex power(const double& n) const;

    /**
     * Power function(Complex)
     * @ written by Teddy van Jerry
     */
    Complex power(const Complex& a) const;

    /**
     * Sqrt function
     * @ written by Teddy van Jerry
     */
    Complex sqrt(const Complex& a) const;

    /**
     * Conjugate function
     * return the conjugate of a Complex number
     * @ written by Panoan
     */
    Complex conjugate() const;

    /**
     * Modulus function
     * return the modulus of a Complex number
     * @ written by Panoan
     */
    Complex modulus() const;

    /**
     * GetReal function
     * return the real part of a Complex number
     * @ written by Teddy van Jerry
     */
    double getReal() const;

    /**
     * GetImaginary function
     * return the imaginary part of a Complex number
     * @ written by Teddy van Jerry
     */
    double getImaginary() const;

    /**
     * argz function
     * return the principle argument angle
     * @ written by Teddy van Jerry
     */
    double argz() const;

    /**
     * unary plus function
     * @ written by Teddy van Jerry
     */
    Complex operator+();

    /**
     * unary minus function
     * @ written by Teddy van Jerry
     */
    Complex operator-();

    /**
     * plus
     * @ written by Teddy van Jerry
     */
    Complex operator+=(const Complex& a);

    /**
     * minus
     * @ written by Teddy van Jerry
     */
    Complex operator-=(const Complex& a);

    /**
     * multiply
     * @ written by Teddy van Jerry
     */
    Complex operator*=(const Complex& a);

    /**
     * divide
     * @ written by Teddy van Jerry
     */
    Complex operator/=(const Complex& a);

    /**
     * power
     * @ written by Teddy van Jerry
     */
    Complex operator^(const double& n);

    /**
     * power
     * @ written by Teddy van Jerry
     */
    Complex operator^=(const double& n);

    /**
     * power
     * @ written by Teddy van Jerry
     */
    Complex operator^(const Complex& n);

    /**
     * power
     * @ written by Teddy van Jerry
     */
    Complex operator^=(const Complex& n);

    /**
     * isequal
     * return true if equal
     * return false if unequal
     * @ written by Teddy van Jerry
     */
    bool operator==(const Complex& a);

    /**
     * !isequal
     * return true if unequal
     * return false if equal
     * @ written by Teddy van Jerry
     */
    bool operator!=(const Complex& a);

    /**
     * assign
     * @ written by Teddy van Jerry
     */
    void operator=(const Complex& a);

    /**
     * assign from other types
     * @ written by Teddy van Jerry
     */
    template<typename T_>
    void operator=(const T_& a);

    /**
     * Convertion
     * return the real part of the complex
     * @ written by Teddy van Jerry
     */
    operator double();

private:
    /**
    * Private members
    * @ written by Teddy van Jerry
    */
    double Real;
    double Imaginary;
};

/**
 * binary plus function
 * @ written by Teddy van Jerry
 */
Complex operator+(const Complex& a, const Complex& b);

/**
 * binary minus function
 * @ written by Teddy van Jerry
 */
Complex operator-(const Complex& a, const Complex& b);

/**
 * multiply function
 * @ written by Teddy van Jerry
 */
Complex operator*(const Complex& a, const Complex& b);

/**
 * divide function
 * @ written by Teddy van Jerry
 */
Complex operator/(const Complex& a, const Complex& b);

/**
 * exp that takes a Complex
 * @ written by Teddy van Jerry
 */
Complex exp(const Complex& n);

/**
 * log that takes a Complex
 * @ written by Teddy van Jerry
 */
Complex log(const Complex& n);

/**
 * sin that takes a Complex
 * @ written by Teddy van Jerry
 */
Complex sin(const Complex& a);

/**
 * cos that takes a Complex
 * @ written by Teddy van Jerry
 */
Complex cos(const Complex& a);

/**
 * Error information
 * @ written by Teddy van Jerry
 */
 //void error(const QString& error_info);


#endif

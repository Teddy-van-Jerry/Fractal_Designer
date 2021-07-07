/* 
 * Project: String_Complex
 * File: String_Complex.h
 * Description: calculate std::string and return std::complex
 * ---------------------------
 * 
 * @author: Teddy van Jerry
 * @licence: MIT Licence
 * @compiler: At least C++11
 * 
 * @version 1.0: 2021/07/05
 * - initial version
 *
 * @version 1.1: 2021/07/07
 * - break the function eval_postorder
 * 
 */

#ifndef STRING_COMPLEX_H
#define STRING_COMPLEX_H

#include <istream>
#include <complex>
#include <string>
#include <stack>
#include <vector>

static const std::vector<std::string> empty_string_list_;           // an empty string list
static const std::vector<std::complex<double>> empty_complex_list_; // an empty complex list
static constexpr std::complex<double> ERROR_COMPLEX { 0, 0 };       // complex of error result
static constexpr std::complex<double> IMAG_I{ 0, 1 };               // the imaginary unit

// If it is defined, error information will be printed.
// #define COMPLEX_CERR_ERROR_

/**
 * \brief type 'var' is used in stack
 *
 * if 'str' is empty, then it is 'num'
 */
struct var {
    std::string str;
    std::complex<double> num;
    // if 'is_var' is true,
    // the number stored in num is the index of the variable
    bool is_var = false;

    // constructors
    var(std::string s) : str(s) {}
    var(std::complex<double> n) : num(n) {}
    explicit var(size_t index) : num(static_cast<double>(index) + 0.5), is_var(true) {}
};


/**
 * \brief Complex number with argument 'real' and 'imag'.
 */
static inline std::complex<double> Comp(double real, double imag);

/**
 * \brief type 'var' is used in stack
 *
 * if 'str' is empty, then it is 'num'
 */
struct var;

/**
 * \brief Change 'c' with 'from' to the corresponding letter in 'to'
 */
inline void _char_replace(char& c, const std::string& from, const std::string& to);

/**
 * \brief Change char of 'str' with 'from' to the corresponding letter in 'to'
 */
inline void _string_replace(std::string& str, const std::string& from, const std::string& to);

/**
 * \brief give error message
 */
inline void __error_msg(std::string* msg, const std::string& word, const char* file_, size_t line_);

/**
 * \brief give error message
 */
#ifndef _error_msg
#define _error_msg(msg, word) __error_msg(msg, word, __FILE__, __LINE__)
#endif

inline int _operator_weight_index(char c);

inline int _operator_weight_compare(char c1, char c2);

inline bool _isoperator(char c);

inline bool _isplus_minus_times_divide(char c);

inline void _unary_to_binary(const std::string& str, std::string& new_str, typename std::string::iterator iter);

/**
 * \brief Pre-edit the
 *
 * The initial string after pre-editing is str,
 * the final postorder result will be stored in vec.
 */
bool _pre_edit(std::string& str);

/**
 * \brief Add a new operator
 *
 * compare the priority of operators,
 * decide to push directly or pop and then compares
 */
inline bool _add_new_oper(const std::string& oper, std::string* msg, std::stack<std::string>& s, std::vector<var>& vec);

/**
 * \brief Change the inorder str to postorder
 *
 * The initial string after pre-editing is str,
 * the final postorder result will be stored in vec.
 */
bool _to_postorder(
    const std::string& str,
    std::string* msg,
    std::vector<var>& vec,
    std::vector<std::string> var_list);

/**
 * \brief implement func in std::string
 */
inline std::complex<double> _func(std::string fun_name, std::complex<double> num, std::string* msg);

/**
 * \brief Evaluate the complex number from the string
 *
 * The second argument msg is optional, return the status.
 * if it is empty, the process is correct;
 * Otherwise, it displays error information.
 */
bool to_postorder(
    std::string str,
    std::string* msg,
    std::vector<var>& post_order,
    std::vector<std::string> var_list);

std::complex<double> eval_postorder(
    std::vector<var> post_order,
    std::vector<std::complex<double>> num_list,
    std::string* msg);

/**
 * \brief Evaluate the complex number from the string
 *
 * The second argument msg is optional, return the status.
 * if it is empty, the process is correct;
 * Otherwise, it displays error information.
 */
std::complex<double> eval(
    std::string str,
    std::vector<std::string> var_list = empty_string_list_,
    std::vector<std::complex<double>> num_list = empty_complex_list_,
    std::string* msg = nullptr);

#endif

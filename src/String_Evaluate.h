/* 
 * Project: String_Evaluate
 * File: String_Evaluate.h
 * Description: calculate std::string and return std::complex
 * ---------------------------
 * 
 * @author: Teddy van Jerry, Pulkit Mittal
 * @licence: MIT Licence
 * @compiler: At least C++11
 * 
 * @version 1.0: 2021/07/05
 * - initial version
 *
 * @version 1.1: 2021/07/08
 * - add bracket check
 *
 * @version 1.2: 2021/07/09
 * - break the function eval_postorder
 * - break into header and source files
 * 
 */

#ifndef STRING_EVALUATE_H
#define STRING_EVALUATE_H

#include <iostream>
#include <complex>
#include <string>
#include <stack>
#include <vector>

static const std::vector<std::string> empty_string_list_;           // an empty string list
static const std::vector<std::complex<double>> empty_complex_list_; // an empty complex list
static constexpr std::complex<double> ERROR_COMPLEX { 0, 0 };       // complex of error result
static constexpr std::complex<double> IMAG_I { 0, 1 };               // the imaginary unit

// If it is defined, error information will be printed.
// #define COMPLEX_CERR_ERROR_

/**
 * \brief type '_var' is used in stack
 *
 * if 'str' is not empty, it stores and operator or function name;
 * else if 'is_var' is true, it stores the index of variable in 'num';
 *     else it stores the complex number in 'num'.
 */
struct _var {
    std::string str;
    std::complex<double> num;
    // if 'is_var' is true,
    // the number stored in num is the index of the variable
    bool is_var = false;

    // constructors
    _var(std::string s);
    _var(std::complex<double> n);
    explicit _var(size_t index);
};

/**
 * \brief change 'c' with 'from' to the corresponding letter in 'to'
 */
inline void _char_replace(char& c, const std::string& from, const std::string& to);

/**
 * \brief change char of 'str' with 'from' to the corresponding letter in 'to'
 */
inline void _string_replace(std::string& str, const std::string& from, const std::string& to);

/**
 * \brief give error message of string evaluation
 *
 * This will include file and line number.
 */
inline void __error_msg(std::string* msg, const std::string& word, const char* file_, size_t line_);

/**
 * \brief give error message of string evaluation
 */
#ifndef _error_msg
#define _error_msg(msg, word) __error_msg(msg, word, __FILE__, __LINE__)
#endif

/**
 * \brief give the index of operator that is used in
 *        function '_operator_weight_compare'
 * \return operator index
 *
 * Function name will have the index 7.
 */
inline int _operator_weight_index(char c);

/**
 * \brief compare the weight of the two operators
 *
 * This is used to decide to push 'c2' directly,
 * or pop 'c1' fisrt and then compare.
 */
inline int _operator_weight_compare(char c1, char c2);

/**
 * \brief check if 'c' is an operator ('+', '-', '*', '/', '^', brackets)
 */
inline bool _isoperator(char c);

/**
 * \brief check if 'c' is '+', '-', '*' or '/'
 */
inline bool _isplus_minus_times_divide(char c);

/**
 * \brief change unary operators into binary operators
 *        i.e. unary plus and unary minus
 *
 * During this time, omitted '*' is added.
 * Functions remain the same.
 */
inline void _unary_to_binary(const std::string& str, std::string& new_str, typename std::string::iterator iter);

/**
 * \brief check the bracket
 * \author Pulkit Mittal
 */
bool _bracket(const std::string& str);

/**
 * \brief pre-edit the string
 *
 * Change the string into normal form.
 */
bool _pre_edit(std::string& str);

/**
 * \brief add a new operator
 *
 * Compare the priority of operators,
 * decide to push directly or pop and then compares.
 */
inline bool _add_new_oper(const std::string& oper, std::string* msg, std::stack<std::string>& s, std::vector<_var>& vec);

/**
 * \brief change the inorder 'str' to postorder
 *
 * The initial string after pre-editing is 'str',
 * the final postorder result will be stored in 'vec'.
 * Need to give 'var_list' first.
 */
bool _to_postorder(
    const std::string& str,
    std::string* msg,
    std::vector<_var>& vec,
    std::vector<std::string> var_list);

/**
 * \brief implement func in std::string
 */
inline std::complex<double> _func(std::string fun_name, std::complex<double> num, std::string* msg);

/**
 * \brief convert string into 'post_order'
 *
 * This function implements validity check
 * besides function '_to_post_order'.
 */
bool to_postorder(
    std::string str,
    std::string* msg,
    std::vector<_var>& post_order,
    std::vector<std::string> var_list);

/**
 * \brief evaluate the postorder vector 'post_order'
 *
 * Need to give the 'num_list'.
 */
std::complex<double> eval_postorder(
    std::vector<_var> post_order,
    const std::vector<std::complex<double>>& num_list,
    std::string* msg);

/**
 * \brief evaluate the complex number from the string
 *
 * The second argument give the variable name (without '$')
 * and the third argument gives the corresponding value.
 * The fourth argument 'msg' is optional, return the status.
 * If it is empty, the process is correct;
 * Otherwise, it displays error information.
 */
std::complex<double> eval(
    std::string str,
    std::vector<std::string> var_list = empty_string_list_,
    std::vector<std::complex<double>> num_list = empty_complex_list_,
    std::string* msg = nullptr);

#endif

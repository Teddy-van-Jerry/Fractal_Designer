/**
 * \file Interpreter.h
 * \brief Interpreter
 *
 * \class Interpreter
 * \include QString Info.h
 * \author Teddy-van-Jerry
 * \version 6.0.0
 * \date 2021-07-18
 *
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <Info.h>

/**
 * \brief the FRD script interpreter
 *
 * \include Interpreter.h
 */
class Interpreter {
public:
    /**
     * \brief default constructor of class Interpreter
     */
    Interpreter();

    /**
     * \brief interpret the text and store into info
     *
     * \details
     * This interprets FRD script,
     * which is programming language to create fractal images and videos
     * in Fractal Designer.
     * After interpretation, is there is no syntax error,
     * the information of the current script will be stores.
     *
     * \param text the QString of Editor text
     * \param info the current info
     * \return whether there is not syntax error
     * \retval true There is no syntax error.
     * \retval false There are syntax errors.
     * \remarks
     * This is a static function, you can call it by:
     * \code{.cpp}bool ok = Interpreter::interpret(text, info);\endcode
     */
    static bool interpret(const QString& text, FRD& info);

    /**
     * \brief interpret strings
     *
     * \return whether there is syntax error
     * \retval true There is no syntax error.
     * \retval false There are syntax errors.
     */
    bool interpret();

private:

    /**
     * \brief set strings
     *
     * \param text the QString of Editor text
     */
    void setStrings(const QString& text);

    /**
     * \brief set info_ptr
     *
     * \param info the current info
     */
    void setInfoPtr(FRD& info);

    /**
     * \brief change comments into whitespace
     * \return whether there are not unfinished comments
     * \retval true There is no comment error.
     * \retval false There exists unfinished comments.
     *
     * \details
     * Replace comments with whitespace,
     * so as not to influence row and col.
     */
    bool removeComments();

    /**
     * \brief get the next non-space char in strings.
     *
     * \return the next char
     * \retval EOF It has already reached the end of text.
     * \remarks
     * This will update the value of row and col.
     */
    char getNext();

    /**
     * \brief read variable defination
     *
     * \return whether there is error in reading variable.
     * \retval true There is no error in reading variable.
     * \retval false There is error in reading variable.
     */
    bool readVar();

    /**
     * \brief read function defination
     *
     * \return whether there is error in reading function.
     * \retval true There is no error in reading function.
     * \retval false There is error in reading function.
     */
    bool readFun();

    bool readClass();

    bool customClass();

    bool readDef();

    QChar nextChar(int* lines = nullptr);

    QStringList strings; /**< text to be interpreted stored by lines */
    FRD* info_ptr;       /**< the pointer to current info */
    int row = 1;         /**< row count that starts at 1 */
    int col = 1;         /**< column count that starts at 1 */

};

#endif // INTERPRETER_H

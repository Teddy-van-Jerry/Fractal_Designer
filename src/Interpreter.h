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
#include <QStack>
#include <Info.h>
#include "String_Evaluate.h"

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
    static bool interpret(const QString& text, FRD_Json& info);

    /**
     * \brief interpret strings
     *
     * \return whether there is syntax error
     * \retval true There is no syntax error.
     * \retval false There are syntax errors.
     */
    bool interpret();

private:

    enum FRD_block_content_ {
        _FRD_BLOCK_BLANK_,
        _FRD_BLOCK_VARIABLE_,
        _FRD_BLOCK_FUNCTION_,
        _FRD_BLOCK_CLASS_
    };

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
    void setInfoPtr(FRD_Json& info);

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

    bool readBlock();

    bool readBlock(FRD_block_content_ content, const QString& name);

    double evalExpr(const QString& expr, const QString& block_name, int start_row, int start_col, bool* ok = nullptr);

    QChar nextChar();

    QString pureName(const QString& name) const;

    QString nextString(QString end_of_string = _empty_string,
                       bool discard_space = false, bool discard_linebreak = false);

    QStringList strings; /**< text to be interpreted stored by lines */
    FRD_Json* info_ptr;  /**< the pointer to current info */
    QChar curr;
    int row = 1;         /**< row count that starts at 1 */
    int col = 0;         /**< column count that starts at 1 */
    bool reach_end = false;

    int block_count = 0;

    static const QString _empty_string;

};

#endif // INTERPRETER_H

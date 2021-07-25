#include "Interpreter.h"

// translation
#define tr QObject::tr
#define info (*info_ptr)

Interpreter::Interpreter() {

}

bool Interpreter::interpret(const QString& text, FRD_Json& json) {
    Interpreter interpreter;
    interpreter.setStrings(text);
    interpreter.setInfoPtr(json);
    return interpreter.interpret();
}

bool Interpreter::interpret() {
    bool error = false;
    error |= !removeComments();
    // error |= !readBlock();
    info.setValue("1.2.3.", "num.X", "Number", 123);
    info.setExistedValue("1.2.3.", "num.X", "", -1234);
    return !error;
}

void Interpreter::setStrings(const QString& text) {
    // split text by line break
    // i.e. store each line in strings
    strings = text.split('\n', Qt::KeepEmptyParts);
}

void Interpreter::setInfoPtr(FRD_Json& ptr) {
    // get the address of ptr
    info_ptr = &ptr;
}

bool Interpreter::removeComments() {
    int comment_row = 0; // the row that "/*" occurs
    int comment_col = 0; // the row that "/*" occurs
    int current_row = 0; // the current line number
    for (auto& line : strings) {
        current_row++;
        if (line.length() < 2) {
            // no chance of being the beginning or end of comments
            // If inside multi-line comments, just clear it out.
            // But here ensures that it has length 1.
            if (comment_row) line.clear();
            line.push_back(' ');
            continue;
        }
        for (auto i = 0; i + 1 != line.length(); i++) {
            if (comment_row) {
                // finding the matching "*/"
                if (line[i] == '*' && line[i + 1] == '/') {
                    line[i] = line[i + 1] = ' ';
                    i++;
                    comment_row = comment_col = 0;
                    continue;
                }
            }
            else {
                // check if it is the beginning of comments
                if (line[i] != '/') continue;
                if (line[i + 1] == '/') {
                    // single-line comments
                    line = line.left(i); // from 0 to i - 1
                    break;
                }
                else if (line[i + 1] == '*') {
                    // multi-line comments
                    comment_row = current_row;
                    comment_col = i + 1; // column number starts at 1
                    line[i] = line[i + 1] = ' ';
                }
            }
        }
    }
    // If comment_row is not set to 0,
    // there are unfinshed comments.
    if (comment_row) {
        info_ptr->addError(_FRD_ERROR_COMMENT_UNFINISHED_,
                           tr("Unfinished Comments"),
                           comment_row, comment_col, 2);
    }
    return !comment_row;
}
/*
bool Interpreter::readVar(FRD_block_content_ content, const QString& name, bool existed, QString new_class_name) {
    qDebug() << "readVar" << name << new_class_name;

    bool ok = true;
    int init_col = col;
    QString var_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
    if (!existed || info.contains(name + "." + var_name)) {
        if (existed && info.contains(name + "." + var_name)) {
            info.addError(_FRD_WARNING_VARIABLE_REDEFINITION_,
                          tr("Redefinition of ") + pureName(var_name),
                          row, init_col, (int)var_name.length());
        }
        else if (!existed) {
            // info.addBaseVar(name, new_class_name);
        }
        auto frd_var = info[name + "." + var_name];
        // auto json_type = frd_var.type();
        QChar c = nextChar();
        if (c == ';') {
            if (existed) {
                // meaningless, just skip
                info.addError(_FRD_WARNING_VARIABLE_DECLARATION_ONLY_,
                              tr("There is only declaration for ") + var_name + tr(" but its value is never used"),
                              row, init_col, (int)var_name.length());
            }
        }
        else if (c == '=') {
            c = nextChar();
            if (c == '{') {
                ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, name + "." + var_name);
            }
            else {
                col--;
                // assignment
                // example: $x = $y;
                if (frd_var.isDouble() || isComplexJsonValue(frd_var) || (!existed && new_class_name == "number")) {
                    // can have maths expression
                    // nextChar();
                    int start_row = row, start_col = col;
                    QString expr = nextString(";", true, true);
                    qDebug() << "rvalue Expr" << expr;
                    bool ok_here;
                    std::complex<double> ret = evalExpr(expr, name, start_row, start_col, &ok_here);
                    qDebug() << "eval result" << ret.real();
                    ok = ok && ok_here;
                    if (frd_var.isDouble() || (!existed && new_class_name == "number")) {
                        info.setValue(name + "." + var_name, "Number", ret.real());
                        qDebug() << "setValue here";
                        // Theoretically, there should be no error.
                    }
                    else {
                        // it is a complex number
                        info.setValue(name + "." + var_name + ".Real", "Real", ret.real());
                        info.setValue(name + "." + var_name + ".Imag", "Imag", ret.imag());
                        // Theoretically, there should be no error.
                    }
                }
                else {
                    // read the rvalue variable
                    QString var_name_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                    // type must match
                    if (info.type(name + "." + var_name) == info.type(name + "." + var_name_r)) {
                        auto error_ = info.setValue(name + "." + var_name,
                                                    info.type(name + "." + var_name),
                                                    name + "." + var_name_r);
                        if (error_ != _FRD_NO_ERROR_) {
                            info.addError(error_,
                                          tr("Assignment from rvalue ") + pureName(var_name_r) + tr(" to lvalue ")
                                          + pureName(var_name) + tr("is invalid"),
                                          row, init_col, var_name_r.length());
                        }
                    }

                    info.addError(_FRD_ERROR_ASSIGNMENT_NO_MATCH_,
                                  tr("Assignment from rvalue ") + pureName(var_name_r) + tr(" to lvalue ")
                                  + pureName(var_name) + tr("is invalid"),
                                  row, init_col, var_name.length());
                    ok = false;
                }
            }
        }
        else if (c == '{') {
            ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, var_name);
        }
    }
    else {
        info_ptr->addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                           tr("Unexpected token: ") + var_name,
                           row, init_col, var_name.length());
        ok = false;
    }
    return ok;
}
*/
bool Interpreter::readFun(FRD_block_content_ content, const QString& name) {
    bool ok = true;
    QString fun_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
    if (fun_name == "CONFIGURE") {
        if (nextChar() != '(') {
            info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                          tr("'(' should follow the function name ") + fun_name,
                          row, col, 1);
            ok = false;
        }
        QStringList params;
        QVector<int> rows, cols;
        QChar c;
        QString param;
        while (!QString(");").contains(c = nextChar())) {
            if (c == ',') {
                params.push_back(param);
                rows.push_back(row);
                cols.push_back(col - param.length());
                param.clear();
            }
            if (c == '$') {
                if (param.isEmpty()) {
                    param.push_back(c);
                }
                else {
                    // lack of a comma
                    info.addError(_FRD_ERROR_FUNCTION_SYNTAX_,
                                  tr("Miss of a comma in function: ") + fun_name,
                                  row, col - 1, 1);
                    ok = false;
                }
            }
            else {
                if (param.isEmpty()) {
                    info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                                  tr("Unexpected token in function: ") + fun_name,
                                  row, col, 1);
                    ok = false;
                }
                else {
                    param.push_back(c);
                }
            }
        }
        if (!param.isEmpty()) {
            params.push_back(param);
            rows.push_back(row);
            cols.push_back(col - param.length());
            param.clear();
        }
    }
    return ok;
}

bool Interpreter::readClass() {
    return true;
}

bool Interpreter::readDef() {
    return true;
}

bool Interpreter::readBlock() {
    return readBlock(_FRD_BLOCK_BLANK_, QString::number(block_count++) + "_");
}

bool Interpreter::readBlock(FRD_block_content_ content, const QString& name) {
    bool ok = true;
    // when it has not reached the end
    QChar curr;
    qDebug() << "Entrance of readBlock";
    while (curr = nextChar(), curr != QChar(0)) { // while (!(reach_end = ((curr = nextChar()) == QChar(EOF)))) {
        qDebug() << "Inside readBlock while";
        if (curr == '}') return ok;
        else if (curr == ';') continue;
        else if (curr == '#') {
            // Marco is not supported in this version.
            row++, col = 0;
        }
        else if (curr == '$') {
            // ok = ok && readVar(content, name, true);
        }
        else if (curr == '@') {
            // class
            QString class_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
            // This version does not support user-defined class.
            if (nextChar() == '$') {
                // ok = ok && readVar(content, name, false, class_name);
            }
            else {
                // unexpected token
                int init_row = row;
                int init_col = col;
                col--; // go back one character for nextString
                int length = nextString(";", true, true).length();
                info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                              tr("Unexpected token after class name ") + class_name,
                              init_row, init_col, length);
                ok = false;
            }
        }
        else if (curr == '%') {
            // function
            ok = ok && readFun(content, name);
        }
        else {
            int init_col = col;
            col--; // go back one character
            QString unexpected_token = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`"); // skip the token
            info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                          tr("Unexpected token: ") + unexpected_token,
                          row, init_col, unexpected_token.length());
            ok = false;
        }
    }
    qDebug() << "Exit of readBlock";
    return ok;
}

std::complex<double> Interpreter::evalExpr(const QString& expr, const QString& block_name, int start_row, int start_col, bool* ok) {
    bool ok_ = true;
    std::vector<std::string> expr_vars;
    std::vector<std::complex<double>> vars_value;
    QVector<int> rows;
    QVector<int> cols;
    int curr_row = start_row;
    int curr_col = start_col;
    for (int i = 0; i != expr.size(); i++) {
        if (expr[i] != '$') {
            if (expr[i] == '\r') curr_col--; // skip '\r'
            else if (expr[i] == '\n') curr_row++, curr_col = 0;
            continue;
        }
        std::string str = block_name.toStdString().append(".");
        while (++i != expr.size() && !expr[i].isSpace() && !QString("+-*/*").contains(expr[i])) {
            str.push_back(expr[i].toLatin1());
        }
        expr_vars.push_back(str);
        rows.push_back(curr_row);
        cols.push_back(curr_col);
        i--; // go back one
        curr_col += str.length() - block_name.length() - 1;
    }
    for (int i = 0; i != rows.size(); i++) { // expr_vars, rows, cols have the same size
        QJsonValue v = 0;
        if (info.getValue(QString::fromStdString(expr_vars[i]), v)) {
            if (v.isDouble()) {
                // a real number
                vars_value.push_back(v.toDouble());
            }
            else if (v.isObject() && v.toObject().contains("Real") && v.toObject().contains("Imag")) {
                // a complex number
                double real_ = v["Real"].toDouble();
                double imag_ = v["Imag"].toDouble();
                std::complex<double> complex_ { real_, imag_ };
                vars_value.push_back(complex_);
            }
            else {
                info.addError(_FRD_ERROR_NON_NUMBER_VALUE_,
                              tr("The variabele ") + pureName(expr[i]) + tr(" can not be used in maths expression."),
                              rows[i], cols[i], expr_vars[i].length() - block_name.length());
                vars_value.push_back(0);
                ok_ = false;
            }
        }
        else {
            info.addError(_FRD_ERROR_UNDEFINED_VARIABLE_,
                          tr("Use of undefined variable ") + pureName(expr[i]),
                          rows[i], cols[i], expr_vars[i].length() - block_name.length());
            vars_value.push_back(0);
            ok_ = false;
        }
    }
    std::complex<double> ret;
    if (ok_) {
        std::string msg;
        ret = eval(expr.toStdString(), expr_vars, vars_value, &msg);
        if (!msg.empty()) {
            // message not empty, there is somethong wrong in calculation
            info.addError(_FRD_ERROR_EVAL_,
                          tr("Error in calculation: ") + QString::fromStdString(msg),
                          start_row, start_col, expr.length());
            ok_ = false;
        }
    }
    if (ok) *ok = ok_; // ok is not a nullptr
    return ret;
}

QChar Interpreter::nextChar() {
    qDebug() << "Entrance of nextChar" << row << col << strings.size();
    while (row <= strings.size()) {
        if (++col - 1 >= strings[row - 1].size()) row++, col = 0; // go to next line
        else if (!strings[row - 1][col - 1].isSpace()) return strings[row - 1][col - 1];
    }
    qDebug() << "Exit of nextChar";
    return QChar(0);
}

QString Interpreter::pureName(const QString& name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.isEmpty()) return "";
    else return names.last();
}

// on the same line
QString Interpreter::nextString(QString end_of_string, bool discard_space, bool discard_linebreak) {
    QString ret;
    do {
        qDebug() << row << col;
        while (++col <= strings[row - 1].size() &&
               (discard_space ? 1 : !strings[row - 1][col - 1].isSpace()) &&
               !end_of_string.contains(strings[row - 1][col - 1]) &&
               strings[row - 1][col - 1] != ';' && strings[row - 1][col - 1] != QChar(0)) {
            ret.push_back(strings[row - 1][col - 1]);
        }
    } // the last one "++row + (col = 0)" aims to prepare for next round
    while (!discard_space && !discard_linebreak && col == strings[row - 1].size() && row + 1 <= strings.size() && ++row + (col = 0));
    col--;
    qDebug() << "Exit of nextString";
    return ret;
}

#undef tr
#undef info

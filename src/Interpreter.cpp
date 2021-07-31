#include "Interpreter.h"

// translation
#define tr QObject::tr
#define info (*info_ptr)

Interpreter::Interpreter() {

}

bool Interpreter::interpret(const QString& text, FRD_Json& json) {
    Interpreter interpreter;
    json.clear();
    interpreter.setStrings(text);
    interpreter.setInfoPtr(json);
    return interpreter.interpret();
}

bool Interpreter::interpret() {
    bool error = false;
    error |= !removeComments();
    error |= !readBlock();
//    info.setValue("1.2.3.", "num.X.", "Number", 123);
//    info.setExistantValue("1.2.3.", "num.X.", "", -1234);
//    qDebug() << info.type("1.2.3.", "num.X.") << info.type("1.2.3.", "num.");
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
                else {
                    line[i] = ' ';
                }
            }
            else {
                // check if it is the beginning of comments
                if (i + 1 >= line.length()) break;
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

bool Interpreter::readVar(FRD_block_content_ content, const QString& block, const QString& name, bool existed, QString new_class_name) {
    qDebug() << "readVar" << name << new_class_name;

    bool ok = true;
    int init_col = col;
    QString var_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
    qDebug() << "readVar" << var_name << block << name;
    if (!existed || info.contains(block, name + var_name) || info.contains(block, name)) {
        if (!existed && info.contains(block, name + var_name)) {
            info.addError(_FRD_WARNING_VARIABLE_REDEFINITION_,
                          tr("Redefinition of ") + var_name,
                          row, init_col, (int)var_name.length());
        }
        else if (!existed) {
            info.setValue(block, name, var_name, new_class_name, QJsonValue::Null);
            qDebug() << "Create an empty value for it" << info.varsToJson();
        }
        // auto frd_var = info.getValue(block, name, var_name);
        auto var_type = info.type(block, name + var_name);
        QChar c = nextChar();
        if (c == ';') {
            if (existed) {
                // meaningless, just skip
                info.addError(_FRD_WARNING_VARIABLE_DECLARATION_ONLY_,
                              tr("There is only declaration for ") + var_name + tr(" but its value is never used"),
                              row, init_col, (int)var_name.length());
            }
        }
        else if (c == ':') {
            int init_row = row;
            int init_col = col;
            if (nextChar() == '=') {
                if (nextChar() == '$') {
                    QString var_name_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                    qDebug() << ":= var_name_r" << var_name_r << "name" << name;
                    qDebug() << info.type(block, name + var_name) << info.type(block, name + var_name_r);
                    // type must match
                    if (info.type(block, name + var_name) == info.type(block, name + var_name_r)) {
                        auto error_ = info.setValue(block,
                                                    name,
                                                    var_name,
                                                    info.type(block, name + var_name),
                                                    info.getValue(block, name, var_name_r));
                        if (error_ != _FRD_NO_ERROR_) {
                            info.addError(error_,
                                          tr("Strict assignment from rvalue ") + var_name_r + tr(" to lvalue ")
                                          + var_name + tr(" is invalid"),
                                          row, init_col, var_name_r.length());
                        }
                    }
                    if (nextChar() != ';') {
                        info.addError(_FRD_ERROR_INVALID_STRICT_ASSIGNMENT_,
                                      "Use of strict assignment := here is invalid, which takes only one variable on the right side",
                                      init_row, init_col, 2);
                        col--;
                        nextString(";}", true, true);
                    }
                }
                else {
                    info.addError(_FRD_ERROR_INVALID_STRICT_ASSIGNMENT_,
                                  "Use of strict assignment := here is invalid, which takes one variable on the right side, and expects $ after :=",
                                  init_row, init_col, 2);
                    nextString(";}", true, true);
                }
            }
            else {
                info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                              "Unecpected toke ':'",
                              init_row, init_col, 2);
                nextString(";}", true, true);
            }
        }
        else if (c == '=') {
            c = nextChar();
            if (c == '{') {
                if (!name.split('.', Qt::SkipEmptyParts).isEmpty()) {
                    // Member variable
                    info.setValue(block, name, var_name, "", QJsonValue::Null);
                }
                ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, block, name + var_name + ".");
            }
            else {
                col--;
                // assignment
                // example: $x = $y;
                if (info.type(block, name + var_name) == "number" || info.type(block, name + var_name) == "complex") {
                    // can have maths expression
                    // nextChar();

                    int start_row = row, start_col = col;
                    QString expr = nextString(";}", true, true);
                    qDebug() << "rvalue Expr" << expr << block << name;
                    bool ok_here;
                    std::complex<double> ret = evalExpr(expr, block, name + ".", start_row, start_col, &ok_here);
                    qDebug() << "eval result" << ret.real();
                    ok = ok && ok_here;
                    if (info.type(block, name + var_name + ".") == "number" || (!existed && new_class_name == "number")) {
                        info.setValue(block, name, var_name, "number", ret.real());
                        qDebug() << "setValue here";
                        // Theoretically, there should be no error.
                    }
                    else {
                        // it is a complex number
                        info.setValue(block, name, var_name + ".Real", "", ret.real());
                        info.setValue(block, name, var_name + ".Imag", "", ret.imag());
                        // Theoretically, there should be no error.
                    }
                }
                else {
                    // read the rvalue variable
                    if (c == '$' && var_type != "formula") {
                        nextChar();
                        QString var_name_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                        qDebug() << "var_name_r" << var_name_r << "name" << name;
                        qDebug() << info.type(block, name + var_name) << info.type(block, name + var_name_r);
                        // type must match
                        if (info.type(block, name + var_name) == info.type(block, name + var_name_r)) {
                            auto error_ = info.setValue(block,
                                                        name,
                                                        var_name,
                                                        info.type(block, name + var_name),
                                                        info.getValue(block, name, var_name_r));
                            if (error_ != _FRD_NO_ERROR_) {
                                info.addError(error_,
                                              tr("Assignment from rvalue ") + var_name_r + tr(" to lvalue ")
                                              + var_name + tr(" is invalid"),
                                              row, init_col, var_name_r.length());
                            }
                        }
                        else {
                            info.addError(_FRD_ERROR_ASSIGNMENT_NO_MATCH_,
                                          tr("Assignment from rvalue ") + pureName(var_name_r) + tr(" to lvalue ")
                                          + pureName(var_name) + tr(" is invalid"),
                                          row, init_col, var_name.length());
                            ok = false;
                        }
                    }
                    else if (var_type == "array") {
                        col--;
                        int init_row = row, init_col = col;
                        QString value = nextString(";}", true, true);
                        QStringList nums = value.split(',', Qt::SkipEmptyParts);
                        QJsonArray arr;
                        bool num_ok;
                        for (const auto& num : nums) {
                            arr.append(num.toDouble(&num_ok));
                            if (!num_ok) {
                                info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                                              num + tr(" is not a number"),
                                              init_row, init_col, 2);
                            }
                        }
                        info.setValue(block, name, var_name, "array", arr);
                    }
                    else if (var_type == "list") {
                        col--;
                        // int init_row = row, init_col = col;
                        QString value = nextString(";}", true, true);
                        QStringList str_list = value.split(',', Qt::SkipEmptyParts);
                        QJsonArray arr;
                        for (const auto& str : str_list) {
                            arr.append(str);
                        }
                        info.setValue(block, name, var_name, "list", arr);
                    }
                    else if (var_type == "bool") {
                        col--;
                        int init_row = row, init_col = col;
                        bool result = true;
                        QString value = nextString(";}", true, true);
                        if (value.simplified() == "false" || value.simplified() == "_FALSE_") {
                            result = false;
                        }
                        else if (value.simplified() == "true" || value.simplified() == "_TRUE_") {
                            result = true;
                        }
                        else {
                            info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                                          tr("This should be an boolean value"),
                                          init_row, init_col, value.length());
                        }
                        info.setValue(block, name, var_name, "bool", result);
                    }
                    else {
                        // value by string

                        col--;
                        QString value = nextString(";}", true, true);
                        value = value.trimmed(); // remove spaces in the front and the end
                        qDebug() << "value by string" << value;
                        auto error_ = info.setValue(block, name, var_name, "", value);
                        qDebug() << info.varsToJson();
                        if (error_ != _FRD_NO_ERROR_) {
                            info.addError(_FRD_ERROR_ASSIGNMENT_NO_MATCH_,
                                          tr("Assignment from rvalue ") + value + tr(" to lvalue ")
                                          + var_name + tr("is invalid"),
                                          row, col, value.length());
                        }
                    }
                }
            }
        }
        else if (c == '{') {
            ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, block + QString::number(block_count++) + ".", var_name);
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

bool Interpreter::readFun(FRD_block_content_ content, const QString& block, const QString& name) {
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
            else if (c == '$') {
                if (param.isEmpty()) {
                    param.push_back('.');
                }
                else {
                    // miss of a comma
                    info.addError(_FRD_ERROR_FUNCTION_SYNTAX_,
                                  tr("Missing comma in function: ") + fun_name,
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
        qDebug() << "Here comes setting param";
        for (const auto& param : params) {
            qDebug() << "useValue" << block << name + param;
            info.useValue(block, name + param);
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
    return readBlock(_FRD_BLOCK_BLANK_, QString::number(block_count++) + ".", ".");
}

bool Interpreter::readBlock(FRD_block_content_ content, const QString& block, const QString& name) {
    bool ok = true;
    // when it has not reached the end
    QChar curr;
    qDebug() << "Entrance of readBlock";
    while (curr = nextChar(), curr != QChar(0)) { // while (!(reach_end = ((curr = nextChar()) == QChar(EOF)))) {
        qDebug() << "Inside readBlock while";
        if (curr == '}') return ok;
        else if (curr == '{') {
            ok = ok && readBlock(_FRD_BLOCK_BLANK_, block + QString::number(block_count++) + ".", name);
        }
        else if (curr == ';') continue;
        else if (curr == '#') {
            // Marco is not supported in this version.
            row++, col = 0;
        }
        else if (curr == '$') {
            ok = ok && readVar(content, block, name);
        }
        else if (curr == '@') {
            // class
            QString class_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
            // This version does not support user-defined class.
            if (nextChar() == '$') {
                ok = ok && readVar(_FRD_BLOCK_VARIABLE_, block, name, false, class_name);
            }
            else {
                // unexpected token
                int init_row = row;
                int init_col = col;
                col--; // go back one character for nextString
                int length = nextString(";}", true, true).length();
                info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                              tr("Unexpected token after class name ") + class_name,
                              init_row, init_col, length);
                ok = false;
            }
        }
        else if (curr == '%') {
            // function
            ok = ok && readFun(content, block, name);
        }
        else {
            int init_col = col;
            qDebug() << strings[row - 1][col - 1];
            col--; // go back one character
            QString unexpected_token = nextString(";}"); // skip the token
            info.addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                          tr("Unexpected token: ") + unexpected_token,
                          row, init_col, unexpected_token.length());
            qDebug() << "Unexpected Token:" << unexpected_token;
            ok = false;
        }
    }
    qDebug() << "Exit of readBlock";
    return ok;
}

std::complex<double> Interpreter::evalExpr(const QString& expr, const QString& block, const QString& name, int start_row, int start_col, bool* ok) {
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
        std::string str;
        while (++i != expr.size() && !expr[i].isSpace() && !QString("+-*/*()[]{}").contains(expr[i])) {
            str.push_back(expr[i].toLatin1());
        }
        expr_vars.push_back(str);
        rows.push_back(curr_row);
        cols.push_back(curr_col);
        i--; // go back one
        curr_col += str.length();
    }

    for (int i = 0; i != rows.size(); i++) { // expr_vars, rows, cols have the same size
        QJsonValue v = 0;
        QString v_type = info.type(block, name + QString::fromStdString(expr_vars[i]));
        v = info.getValue(block, name, QString::fromStdString(expr_vars[i]));
        if (v_type == "number") {
            // a real number
            vars_value.push_back(v.toDouble());
        }
        else if (v_type == "complex") {
            // a complex number
            double real_ = v["Real"].toDouble();
            double imag_ = v["Imag"].toDouble();
            std::complex<double> complex_ { real_, imag_ };
            vars_value.push_back(complex_);
        }
        else {
            info.addError(_FRD_ERROR_NON_NUMBER_VALUE_,
                          tr("The variabele ") + pureName(expr[i]) + tr(" can not be used in maths expression."),
                          rows[i], cols[i], expr_vars[i].length());\
            qDebug() << tr("The variabele ") + pureName(expr[i]) + tr(" can not be used in maths expression.");
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
            qDebug() << tr("Error in calculation: ") + QString::fromStdString(msg);
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
        if (col < 0) col = 0;
        if (!ret.isEmpty()) {
            ++row;
            col = 0;
            ret.push_back('\n');
        }
        while (++col <= strings[row - 1].size() &&
               (discard_space ? 1 : !strings[row - 1][col - 1].isSpace()) &&
               !end_of_string.contains(strings[row - 1][col - 1]) &&
               strings[row - 1][col - 1] != ';' && strings[row - 1][col - 1] != QChar(0)) {
            ret.push_back(strings[row - 1][col - 1]);
        }
    } // the last one "++row + (col = 0)" aims to prepare for next round
    while (discard_space && discard_linebreak && col > strings[row - 1].size() && row + 1 <= strings.size());
    col--;
    qDebug() << "Exit of nextString";
    return ret;
}

#undef tr
#undef info

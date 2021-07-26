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

    struct bracket_ {
        QChar type;
        int row, col;
    };

    bool error = false;
    error |= !removeComments();

    QStack<bracket_> brackets;
    while (1) {

    }
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

bool Interpreter::readVar() {
    return true;
}

bool Interpreter::readFun() {
    return true;
}

bool Interpreter::readClass() {
    return true;
}

bool Interpreter::readDef() {
    return true;
}

bool Interpreter::readBlock() {
    return readBlock(_FRD_BLOCK_BLANK_, QString::number(block_count++));
}

bool Interpreter::readBlock(FRD_block_content_ content, const QString& name) {
    bool ok = true;
    // when it has not reached the end
    while (!(reach_end = ((curr = nextChar()) == QChar(EOF)))) {
        if (curr == '}') return ok;
        else if (curr == ';') continue;
        else if (curr == '#') {
            // marco
            row++, col = 0;
        }
        else if (curr == '$') {
            int init_col = col;
            QString var_name = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
            if (info_ptr->contains(name + "." + var_name)) {
                auto frd_var = info[name + "." + var_name];
                auto json_type = frd_var.type();
                QChar c = nextChar();
                if (c == ';') {
                    // meaningless, just skip
                    info_ptr->addError(_FRD_WARNING_DECLARATION_ONLY_,
                                       tr("There is only declaration for ") + var_name + tr(" but its value is never used."),
                                       row, init_col, (int)var_name.length());
                }
                else if (c == '=') {
                    c = nextChar();
                    if (c == '{') {
                        ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, name + "." + var_name);
                    }
                    else {
                        // assignment
                        // example: $x = $y;
                        if (json_type == QJsonValue::Double) {
                            // read the rvalue variable
                            QString var_name_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                            auto frd_var_r = info[var_name_r];
                            auto json_type_r = frd_var.type();
                            if (json_type_r == QJsonValue::Double) {
                                // It can be a maths expression, need to call string_evaluate
                                nextChar();
                                int start_row = row, start_col = col;
                                QString expr = nextString(";", true, true);
                                bool ok_here;
                                evalExpr(expr, name, start_row, start_col, &ok_here);
                                ok = ok && ok_here;
                            }
                            else {
                                info_ptr->addError(_FRD_ERROR_ASSIGNMENT_NO_MATCH_,
                                                   tr("Assignment from rvalue ") + pureName(var_name_r) + tr(" to lvalue ")
                                                   + pureName(var_name) + tr("is invalid"),
                                                   row, init_col, var_name.length());
                            }
                        }
                        else {
                            // TODO
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
        }
        else if (curr == '@') {

        }
        else if (curr == '%') {
            // function
        }
        else {
            int init_col = col;
            col--; // go back one character
            QString unexpected_token = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`"); // skip the token
            info_ptr->addError(_FRD_ERROR_UNEXPECTED_TOKEN_,
                               tr("Unexpected token: ") + unexpected_token,
                               row, init_col, unexpected_token.length());
        }
    }
    return ok;
}

double Interpreter::evalExpr(const QString& expr, const QString& block_name, int start_row, int start_col, bool* ok) {
    std::vector<std::string> expr_vars;
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
        while (++i != expr.size() && !expr[i].isSpace() && !QString("+-*/*").contains(expr[i])) {
            str.push_back(expr[i].toLatin1());
        }
        expr_vars.push_back(str);
        rows.push_back(curr_row);
        cols.push_back(curr_col - str.length());
    }
    return 0;
}

QChar Interpreter::nextChar() {
    while (row <= strings.size()) {
        if (++col - 1 == strings[row - 1].size()) row++, col = 0; // go to next line
        else if (!strings[row - 1][col - 1].isSpace()) return strings[row - 1][col - 1];
    }
    return QChar(EOF);
}

QString Interpreter::pureName(const QString& name) const {
    QStringList names =  name.split('.', Qt::SkipEmptyParts);
    if (names.isEmpty()) return "";
    else return names.last();
}

// on the same line
QString Interpreter::nextString(QString end_of_string, bool discard_space, bool discard_linebreak) {
    QString ret;
    do {
        while (++col != strings[row - 1].size() &&
               (discard_space ? 1 : !strings[row - 1][col - 1].isSpace()) &&
               !end_of_string.contains(strings[row - 1][col - 1]) &&
               strings[row - 1][col - 1] != ';') {
            ret.push_back(strings[row - 1][col - 1]);
        }
    } // the last one "(col = 0), ++row" aims to prepare for next round
    while (!discard_space && !discard_linebreak && col == strings[row + 1].size() && row != strings.size() && (col = 0), ++row);
    col--;
    return ret;
}

#undef tr
#undef info

#include "Interpreter.h"

Interpreter::Interpreter() {

}

bool Interpreter::interpret(const QString& text, FRD& info) {
    Interpreter interpreter;
    interpreter.setStrings(text);
    interpreter.setInfoPtr(info);
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

void Interpreter::setInfoPtr(FRD& ptr) {
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
        info_ptr->Errors.push_back({_FRD_ERROR_COMMENT_UNFINISHED_, comment_row, comment_col});
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
            if (info_ptr->Vars.contains(name + "." + var_name)) {
                FRD_var_ frd_var = info_ptr->Vars[name + "." + var_name];
                FRD_class_ frd_class = frd_var.varClass;
                QChar c = nextChar();
                if (c == ';') {
                    // meaningless, just skip
                    info_ptr->Errors.push_back({_FRD_WARNING_DECLARATION_ONLY_, row, init_col, (int)var_name.length()});
                }
                else if (c == '=') {
                    c = nextChar();
                    if (c == '{') {
                        ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, name + "." + var_name);
                    }
                    else {
                        // assignment
                        // example: $x = $y;
                        if (frd_class == _FRD_CLASS_INT_) {
                            // read the rvalue variable
                            c = nextChar();
                            if (c == '~') {
                                // global variable
                                QString var_name_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                                FRD_var_ frd_var = info_ptr->Vars[var_name_r];
                                // TODO
                            }
                            else {
                                col--; // go back one character
                                QString frd_var_r = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`");
                                // TODO
                            }
                        }
                    }
                }
                else if (c == '{') {
                    ok = ok && readBlock(_FRD_BLOCK_VARIABLE_, var_name);
                }
            }
            else {
                info_ptr->Errors.push_back({_FRD_ERROR_UNEXPECTED_TOKEN_, row, init_col, (int)var_name.length()});
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
            int length = nextString("!@#$%^&*=+-/?:;()[]{}<>\\'\"~`").length(); // skip the token
            info_ptr->Errors.push_back({_FRD_ERROR_UNEXPECTED_TOKEN_, row, init_col, length});
        }
    }
}

QChar Interpreter::nextChar() {
    while (row <= strings.size()) {
        if (++col - 1 == strings[row - 1].size()) row++, col = 0; // go to next line
        else if (!strings[row - 1][col - 1].isSpace()) return strings[row - 1][col - 1];
    }
    return QChar(EOF);
}

// on the same line
QString Interpreter::nextString(QString end_of_string, bool discard_space) {
    QString ret;
    while (++col != strings[row + 1].size() &&
           (discard_space ? 1 : !strings[row + 1][col + 1].isSpace()) &&
           !end_of_string.contains(strings[row - 1][col - 1]) &&
           strings[row - 1][col - 1] != ';') {
        ret.push_back(strings[row + 1][col + 1]);
    }
    col--;
    return ret;
}

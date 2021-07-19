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
    bool error = false;
    error |= !removeComments();
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
    size_t comment_row = 0; // the row that "/*" occurs
    size_t comment_col = 0; // the row that "/*" occurs
    size_t current_row = 0; // the current line number
    for (auto& line : strings) {
        current_row++;
        if (line.length() < 2) {
            // no chance of being the beginning or end of comments
            // If inside multi-line comments, just clear it out.
            if (comment_row) line.clear();
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
        info_ptr->errors.push_back({_FRD_ERROR_COMMENT_UNFINISHED_, comment_row, comment_col});
    }
    return !comment_row;
}

bool Interpreter::readVar() {
    return true;
}

bool Interpreter::readFun() {
    return true;
}

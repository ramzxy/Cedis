
#include "parser.h"

#include <iostream>
#include <bits/ostream.tcc>

void parser::addToBuffer(const std::vector<uint8_t> &data) {
    buffer_.insert(buffer_.end(), data.begin(), data.end());
}

bool parser::isCommandValid() {
    if (buffer_.size() < 2) return false;

    if ( buffer_[buffer_.size() - 1] == '\n'
    && buffer_[buffer_.size() - 2] == '\r')
        return true;
    return false;
}
//TODO: Fix parser
std::vector<std::string> parser::parse() {
    char prefix = buffer_[0];
    switch (prefix)
    {
    case '+':
        simpleStringParse();
        break;

    case '-':
        errorParse();
        break;
    case ':':
        intParse();
        break;

    case '*':
        arrayParse();
        break;

    case '$':
        bulkStringParse();
        break;

    default:
        return {"Prefix Error"};
    }

    std::vector<std::string> commands;
    size_t pindex = 0;

    std::string sign(1, static_cast<char>(buffer_[0]));
    std::cout << "\nreceived sign: " << sign << std::endl;

    while (buffer_[pindex] != '\r') pindex++;
    std::string command(buffer_.begin() + 1, buffer_.begin() + pindex);
    commands.push_back(command);
    return commands;
}